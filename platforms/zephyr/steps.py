from dataclasses import dataclass
from pathlib import Path
from typing import Any

import kconfiglib
from mashumaro import DataClassDictMixin
from py_app_dev.core.exceptions import UserNotificationException
from pypeline.domain.external_project import ExternalProject
from pypeline.domain.pipeline import PipelineStep
from yanga.cmake.cmake_backend import CMakeElement, CMakeTargetLinkLibraries, LinkLibrary, LinkScope
from yanga.cmake.create_executable import CreateExecutableCMakeGenerator
from yanga_core.domain.execution_context import ExecutionContext

#: Zephyr's application configuration directory: prj.conf, Kconfig, boards/. The CMake
#: source directory is the repository root, like every other platform.
APP_CONFIG_DIR = Path("platforms/zephyr/app")


def zephyr_base(execution_context: ExecutionContext) -> Path:
    """The Zephyr checkout WestInstall registered, wherever the workspace layout put it."""
    zephyr = next((project for project in execution_context.data_registry.find_data(ExternalProject) if project.name == "zephyr"), None)
    if zephyr is None:
        raise UserNotificationException("No west project named 'zephyr' was installed. Check platforms/zephyr/west.yaml.")
    return zephyr.path


def variant_conf_file(execution_context: ExecutionContext) -> Path:
    """The variant's feature selection in the form Zephyr's Kconfig merge accepts."""
    return execution_context.spl_paths.variant_build_dir / "variant.conf"


class ZephyrSetup(PipelineStep[ExecutionContext]):
    """Publishes ZEPHYR_BASE from the Zephyr checkout WestInstall registered, so west finds the workspace from any directory."""

    def run(self) -> None:
        return None

    def get_inputs(self) -> list[Path]:
        return []

    def get_outputs(self) -> list[Path]:
        return []

    def get_name(self) -> str:
        return self.__class__.__name__

    def update_execution_context(self) -> None:
        # Here rather than in run(): called on every invocation, including cache hits.
        self.execution_context.add_env_vars({"ZEPHYR_BASE": zephyr_base(self.execution_context).as_posix()})


class GenerateZephyrVariantConfig(PipelineStep[ExecutionContext]):
    """
    Writes the variant's minimal Kconfig fragment for Zephyr.

    The variant's config.txt is a full saved .config, so it also assigns derived symbols
    without a prompt, which Zephyr's Kconfig merge rejects. The minimal config holds only
    the user's choices that differ from the defaults, and Zephyr computes the rest from the
    same feature model.
    """

    def __init__(self, execution_context: ExecutionContext, group_name: str, config: dict[str, Any] | None = None) -> None:
        super().__init__(execution_context, group_name, config)
        self.kconfig = kconfiglib.Kconfig((self.project_root_dir / "KConfig").as_posix(), warn_to_stderr=False)
        self.output_file = variant_conf_file(execution_context)

    @property
    def features_file(self) -> Path:
        features_file = self.execution_context.features_selection_file
        if not features_file:
            raise UserNotificationException("The selected variant has no features_selection_file.")
        return features_file

    def run(self) -> None:
        self.kconfig.load_config(self.features_file.as_posix())
        self.output_file.parent.mkdir(parents=True, exist_ok=True)
        self.kconfig.write_min_config(self.output_file.as_posix())

    def get_inputs(self) -> list[Path]:
        return [self.features_file, *(Path(name) for name in self.kconfig.kconfig_filenames)]

    def get_outputs(self) -> list[Path]:
        return [self.output_file]

    def get_name(self) -> str:
        return self.__class__.__name__

    def update_execution_context(self) -> None:
        pass


@dataclass
class ZephyrBuildConfig(DataClassDictMixin):
    #: Zephyr board name, e.g. esp32h2_devkitm/esp32h2
    board: str
    #: ZEPHYR_TOOLCHAIN_VARIANT: host or cross-compile
    toolchain: str
    #: The poks app providing the cross toolchain. Its name is also the compiler prefix.
    toolchain_app: str | None = None
    #: Sysroot relative to the toolchain root, when Zephyr's own detection composes a wrong path.
    sysroot: str | None = None


class ZephyrBuild(PipelineStep[ExecutionContext]):
    """Builds or flashes the variant with west; `--target flash` flashes, any other target is passed to `west build -t`."""

    def __init__(self, execution_context: ExecutionContext, group_name: str, config: dict[str, Any] | None = None) -> None:
        super().__init__(execution_context, group_name, config)
        self.zephyr_config = ZephyrBuildConfig.from_dict(config or {})

    @property
    def build_dir(self) -> Path:
        return self.execution_context.spl_paths.variant_build_dir

    def run(self) -> None:
        target = self.execution_context.user_request.target
        target_name = str(target) if target else "all"
        if target_name == "flash":
            command = ["west", "flash", "-d", self.build_dir.as_posix()]
        else:
            command = ["west", "build", "-b", self.zephyr_config.board, "-d", self.build_dir.as_posix(), self.project_root_dir.as_posix()]
            if target_name != "all":
                command += ["-t", target_name]
            command += ["--", *self.get_cmake_args()]
        self.execution_context.create_process_executor(command, cwd=self.project_root_dir).execute()

    def get_cmake_args(self) -> list[str]:
        """Passed per build rather than exported: Zephyr caches them per build directory and gives environment variables the lowest precedence."""
        args = [
            # Cached per build directory by Zephyr, so a moved workspace would otherwise keep the old path.
            f"-DZEPHYR_BASE={zephyr_base(self.execution_context).as_posix()}",
            f"-DAPPLICATION_CONFIG_DIR={(self.project_root_dir / APP_CONFIG_DIR).as_posix()}",
            # The feature model is the Kconfig root; it pulls Zephyr's tree in with `osource`. Named
            # explicitly because Zephyr's default lookup wants a file called exactly `Kconfig`.
            f"-DKCONFIG_ROOT={(self.project_root_dir / 'KConfig').as_posix()}",
            f"-DVARIANT={self.execution_context.variant_name}",
            f"-DPLATFORM={self.execution_context.platform.name if self.execution_context.platform else ''}",
            f"-DZEPHYR_TOOLCHAIN_VARIANT={self.zephyr_config.toolchain}",
        ]
        args.append(f"-DEXTRA_CONF_FILE={variant_conf_file(self.execution_context).as_posix()}")
        if self.zephyr_config.toolchain != "cross-compile":
            return args

        app = self.zephyr_config.toolchain_app
        if not app:
            raise UserNotificationException("A cross-compile toolchain needs 'toolchain_app' in the ZephyrBuild config.")
        # PoksInstall published every installed app's bin directory; the toolchain root is one level up.
        bin_dirs = [directory for directory in self.execution_context.install_dirs if directory.parent.parent.name == app]
        if not bin_dirs:
            raise UserNotificationException(f"Toolchain '{app}' is not installed. It must be listed in platforms/zephyr/poks.json.")
        root = bin_dirs[0].parent
        args += [
            f"-DCROSS_COMPILE={(root / 'bin' / app).as_posix()}-",
            # Without this the toolchain's libc support is never probed, and picolibc silently disappears from Kconfig.
            f"-DCROSS_COMPILE_TOOLCHAIN_PATH={root.as_posix()}",
        ]
        if self.zephyr_config.sysroot:
            args.append(f"-DSYSROOT_DIR={(root / self.zephyr_config.sysroot).as_posix()}")
        return args

    def get_needs_dependency_management(self) -> bool:
        """Ninja already knows what is up to date."""
        return False

    def get_inputs(self) -> list[Path]:
        return []

    def get_outputs(self) -> list[Path]:
        return []

    def get_name(self) -> str:
        return self.__class__.__name__

    def update_execution_context(self) -> None:
        pass


class ZephyrCMakeGenerator(CreateExecutableCMakeGenerator):
    """
    Zephyr owns the executable: the component libraries are linked into its `app` library.

    Each library links `zephyr_interface`, the target Zephyr keeps every compiler setting on;
    CMake usage requirements flow from library to consumer only, so `app` linking it is not
    enough. Not `zephyr_library_named()`: that registers only while Zephyr's own tree is
    processed, and from here it warns and drops the library. The object libraries linked into
    the static `app` are archived into it, which Zephyr then links whole.
    """

    component_link_libraries = (LinkLibrary("zephyr_interface"),)

    @property
    def executable_target_name(self) -> str:
        return "app"

    def create_executable_elements(self, component_library_targets: list[str]) -> list[CMakeElement]:
        # Keyword form: Zephyr already used it on `app`, and CMake forbids mixing the two signatures.
        return [CMakeTargetLinkLibraries("app", component_library_targets, scope=LinkScope.PRIVATE)]
