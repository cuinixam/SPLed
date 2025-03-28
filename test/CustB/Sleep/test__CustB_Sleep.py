from pathlib import Path
import pytest
from spl_core.test_utils.base_variant_test_runner import BaseVariantTestRunner
from spl_core.test_utils.spl_build import SplBuild


class Test_CustB__Sleep(BaseVariantTestRunner):
    @property
    def component_paths(self):
        return [
            # Path("src/spled"),
            Path("src/power_signal_processing"),
            Path("src/light_controller"),
            Path("src/keyboard_interface"),
            # Path("src/console_interface"),
            Path("src/main_control_knob"),
            Path("src/brightness_controller"),
        ]

    @property
    def expected_build_artifacts(self):
        return [Path("spled.exe"), Path("compile_commands.json")]

    @pytest.mark.static_analysis
    def test_static_analysis(self):
        # Arrange
        spl_build: SplBuild = SplBuild(variant=self.variant, build_kit="prod")

        # Act
        assert 0 == spl_build.execute(target="static_analysis")

        # Assert
        self.assert_artifact_exists(
            dir=spl_build.build_dir,
            artifact=Path("reports/static_analysis/cppcheck/index.html"),
        )

        self.assert_artifact_exists(
            dir=spl_build.build_dir,
            artifact=Path("reports/static_analysis/cppcheck/index.xml"),
        )
