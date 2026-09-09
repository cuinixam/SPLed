import sys
from pathlib import Path

import pytest

from yanga_core.commands.run import RunCommand, RunCommandConfig


class Test_Spa:
    variant_name = "Spa"

    @pytest.mark.parametrize(
        ("platform", "target"),
        [
            ("pc_terminal", "report"),
            ("pc_gui", "report"),
            pytest.param("arduino_uno_r3", "report", marks=pytest.mark.skipif(sys.platform == "darwin", reason="poks has no avr-gcc for macOS")),
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
