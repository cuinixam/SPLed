from pathlib import Path
import pytest
from spl_core.test_utils.base_variant_test_runner import BaseVariantTestRunner
from spl_core.test_utils.spl_build import SplBuild


class Test_Sleep(BaseVariantTestRunner):
    @property
    def component_paths(self):
        return [
            # Path("components/spled"),
            Path("components/power_signal_processing"),
            Path("components/light_controller"),
            Path("components/power_button"),
            # Path("components/console_interface"),
            Path("components/main_control_knob"),
            Path("components/brightness_controller"),
        ]

    @property
    def expected_build_artifacts(self):
        return [Path("spled.exe"), Path("compile_commands.json")]

    @pytest.mark.build
    def test_build(self):
        super().test_build(build_type="Debug")

    @pytest.mark.unittests
    def test_unittests(self):
        super().test_unittests(build_type="Debug")

    @pytest.mark.reports
    def test_reports(self):
        super().test_reports(build_type="Debug")

    @pytest.mark.static_analysis
    def test_static_analysis(self):
        # Arrange
        spl_build: SplBuild = SplBuild(
            variant=self.variant, build_kit="prod", build_type="Debug"
        )

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
