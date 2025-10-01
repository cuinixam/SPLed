from pathlib import Path

import pytest

from yanga.commands.run import RunCommand, RunCommandConfig


class Test_BaseDev:
    variant_name = "Base/Dev"

    @pytest.mark.report
    def test_report(self):
        # Arrange
        config = RunCommandConfig(
            project_dir=Path.cwd(),
            platform="gtest",
            variant_name=self.variant_name,
            not_interactive=True,
        )

        # Act
        result = RunCommand().do_run(config)

        # Assert
        assert result == 0, "Building variant failed"
