from pathlib import Path

import pytest

from yanga.commands.run import RunCommand, RunCommandConfig


class Test_Spa:
    variant_name = "Spa"

    @pytest.mark.parametrize("platform", ["pc_terminal", "arduino_uno_r3", "gtest"])
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
