from pathlib import Path

import pytest

from yanga_core.commands.run import RunCommand, RunCommandConfig


class Test_BlinkLed:
    variant_name = "BlinkLed"

    @pytest.mark.parametrize("platform", ["arduino_uno_r3"])
    def test_build(self, platform: str):
        # Arrange
        config = RunCommandConfig(
            project_dir=Path.cwd(),
            platform=platform,
            variant_name=self.variant_name,
            not_interactive=True,
        )

        # Act
        result = RunCommand().do_run(config)

        # Assert
        assert result == 0, "Building variant failed"
