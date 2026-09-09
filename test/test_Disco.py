import subprocess
import sys
from pathlib import Path

import pytest

from spled_lib import POWER_BUTTON_KEY, Variant, build_dir, find_library
from yanga_core.commands.run import RunCommand, RunCommandConfig
from yanga_core.domain.spl_paths import SPLPaths

VARIANT_NAME = "Disco"
# POWER_BUTTON_PRESS_DEBOUNCE, from components/power_button/src/power_button.h.
PRESS_DEBOUNCE_STEPS = 10
LIGHT_OFF = "#000000"
# CONFIG_COLOR_GREEN at the default brightness of 128.
LIGHT_GREEN = "#008000"


@pytest.fixture(scope="module")
def variant() -> Variant:
    """
    Build the variant as a shared library and bind to it.

    Requesting this fixture is what orders the build before the test that drives the
    library; declaration order in the file would not survive `-k` or a single node id.
    """
    config = RunCommandConfig(
        project_dir=Path.cwd(),
        platform="pc_gui",
        variant_name=VARIANT_NAME,
        not_interactive=True,
        target="build",
    )
    assert RunCommand().do_run(config) == 0, "Building the shared library failed"

    return Variant(find_library(build_dir(Path.cwd(), VARIANT_NAME), VARIANT_NAME))


@pytest.fixture(scope="module")
def native_sim() -> Path:
    """Build the variant for Zephyr's native_sim; same ordering reason as `variant`."""
    config = RunCommandConfig(
        project_dir=Path.cwd(),
        platform="zephyr_sim",
        variant_name=VARIANT_NAME,
        not_interactive=True,
    )
    assert RunCommand().do_run(config) == 0, "Building for native_sim failed"

    # The Zephyr platforms declare no build type, so the variant build dir has no such level.
    return SPLPaths(Path.cwd(), VARIANT_NAME, "zephyr_sim", None).variant_build_dir / "zephyr" / "zephyr.exe"


class Test_Disco:
    variant_name = VARIANT_NAME

    @pytest.mark.parametrize(
        ("platform", "target"),
        [
            ("pc_terminal", "report"),
            ("pc_gui", "report"),
            ("gtest", "report"),
            # The riscv64 toolchain comes from poks on every OS; native_sim needs a Linux host.
            pytest.param("zephyr_sim", "all", marks=pytest.mark.skipif(sys.platform != "linux", reason="native_sim is Linux-only")),
            ("zephyr_esp32h2", "all"),
        ],
    )
    def test_build(self, platform: str, target: str):
        # Arrange
        config = RunCommandConfig(
            project_dir=Path.cwd(),
            platform=platform,
            variant_name=self.variant_name,
            not_interactive=True,
            target=target,
        )

        # Act
        result = RunCommand().do_run(config)

        # Assert
        assert result == 0, "Building variant failed"

    def test_holding_the_power_button_turns_the_light_green(self, variant: Variant):
        """Drives the built library the way the GUI does: button in, LED colour out."""
        # Arrange
        variant.set_button(POWER_BUTTON_KEY, True)

        # Act
        for _ in range(PRESS_DEBOUNCE_STEPS - 1):
            variant.step()

        # Assert
        assert variant.led_colour() == LIGHT_OFF, "The light must stay off until the press is debounced"
        variant.step()
        assert variant.led_colour() == LIGHT_GREEN

    @pytest.mark.skipif(sys.platform != "linux", reason="native_sim is Linux-only")
    def test_boots_on_native_sim(self, native_sim: Path):
        """Two seconds of simulated time, then the simulator stops on its own (`--stop-at`)."""
        # Act
        result = subprocess.run([native_sim, "--stop-at=2"], capture_output=True, text=True, check=True, timeout=60)

        # Assert
        assert "Booting Zephyr OS" in result.stdout + result.stderr
