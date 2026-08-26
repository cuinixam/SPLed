from pathlib import Path

import pytest

from spled_lib import POWER_BUTTON_KEY, Variant, build_dir, find_library
from yanga_core.commands.run import RunCommand, RunCommandConfig

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


class Test_Disco:
    variant_name = VARIANT_NAME

    @pytest.mark.parametrize("platform", ["pc_terminal", "pc_gui", "gtest"])
    def test_build(self, platform: str):
        # Arrange
        config = RunCommandConfig(
            project_dir=Path.cwd(),
            platform=platform,
            variant_name=self.variant_name,
            not_interactive=True,
            target="report",
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
