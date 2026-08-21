"""
Build a variant as a shared library instead of an executable, so that a Python GUI
can load it with ctypes and drive the super loop itself.

This lives in the platform folder rather than in yanga because yanga's CMake backend
models only OBJECT and INTERFACE libraries. A project-local generator is enough to
teach it a new artifact kind, which is the point worth showing: the platform, not the
tool, decides what a variant is built into.
"""

from yanga.cmake.cmake_backend import (
    CMakeAddLibrary,
    CMakeComment,
    CMakeContent,
    CMakeCustomTarget,
    CMakeElement,
    CMakeSetTargetProperties,
)
from yanga.cmake.create_executable import CreateExecutableCMakeGenerator
from yanga_core.domain.execution_context import UserRequestTarget, UserVariantRequest

SHARED_LIBRARY_TARGET = "${PROJECT_NAME}"


class CreateSharedLibraryCMakeGenerator(CreateExecutableCMakeGenerator):
    """
    Links the variant's component object libraries into a shared library.

    Only the variant-level artifact differs from CreateExecutableCMakeGenerator; the
    per-component object libraries, include directories and component targets are
    inherited unchanged.
    """

    def create_variant_cmake_elements(self) -> list[CMakeElement]:
        elements: list[CMakeElement] = []
        if self.config_obj.use_global_includes:
            elements.append(self.get_include_directories())
        else:
            elements.append(CMakeComment("Use global includes for all components disabled."))

        component_library_targets = [CMakeAddLibrary(component.name).target_name for component in self.execution_context.components]

        # Raw CMake: CMakeAddLibrary has no SHARED type and cannot link libraries.
        # Everything below this is expressible with the typed backend.
        elements.append(CMakeContent(f"add_library({SHARED_LIBRARY_TARGET} SHARED)"))
        elements.append(CMakeContent(f"target_link_libraries({SHARED_LIBRARY_TARGET} {' '.join(component_library_targets)})"))
        elements.append(
            CMakeSetTargetProperties(
                SHARED_LIBRARY_TARGET,
                {
                    # The GUI looks the library up by variant name, so drop the
                    # platform's "lib" prefix and keep the file name identical
                    # on Windows, Linux and macOS.
                    "OUTPUT_NAME": self.variant_name or "spled",
                    "PREFIX": '""',
                    # A DLL exports nothing unless asked; the .so and .dylib do by default.
                    "WINDOWS_EXPORT_ALL_SYMBOLS": "ON",
                },
            )
        )
        elements.append(
            CMakeCustomTarget(
                name=UserVariantRequest(self.variant_name, UserRequestTarget.BUILD).target_name,
                description=f"Build variant {self.variant_name}",
                commands=[],
                depends=[SHARED_LIBRARY_TARGET],
            )
        )
        elements.append(
            CMakeCustomTarget(
                name=UserVariantRequest(self.variant_name, UserRequestTarget.COMPILE).target_name,
                description=f"Compile variant {self.variant_name}",
                commands=[],
                depends=component_library_targets,
            )
        )
        return elements
