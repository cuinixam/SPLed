from pathlib import Path

import pytest

from yanga.commands.run import RunCommand, RunCommandConfig


class Test_Spa:
    @pytest.mark.build
    @pytest.mark.parametrize("platform", ["win_exe", "arduino_uno_r3"])
    def test_build(self, platform: str):
        # Arrange
        config = RunCommandConfig(
            project_dir=Path.cwd(),
            platform=platform,
            variant_name="Spa",
            not_interactive=True,
        )

        # Act
        result = RunCommand().do_run(config)

        # Assert
        assert result == 0, "Building variant failed"

    @pytest.mark.report
    def test_report(self):
        # Arrange
        config = RunCommandConfig(
            project_dir=Path.cwd(),
            platform="gtest",
            variant_name="Spa",
            not_interactive=True,
        )

        # Act
        result = RunCommand().do_run(config)

        # Assert
        assert result == 0, "Building variant failed"
