from pathlib import Path
import pytest
from spl_core.test_utils.base_variant_test_runner import BaseVariantTestRunner
from spl_core.test_utils.spl_build import SplBuild


class Test_Disco(BaseVariantTestRunner):
    @property
    def component_paths(self):
        return [
            # Path("components/spled"),
            Path("components/power_signal_processing"),
            Path("components/light_controller"),
            Path("components/power_button"),
            # Path("components/console_interface"),
            Path("components/main_control_knob"),
        ]

    @property
    def expected_build_artifacts(self):
        return [Path("spled.exe"), Path("compile_commands.json")]

    @pytest.mark.parametrize(
        ("build_type"),
        [
            pytest.param("Debug", marks=pytest.mark.build_debug),
            pytest.param("Release", marks=pytest.mark.build_release),
        ],
    )
    def test_build(self, build_type):
        super().test_build(build_type=build_type)

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
        ret_code = spl_build.execute(target="static_analysis")

        # Assert
        assert ret_code == 0

        self.assert_artifact_exists(
            dir=spl_build.build_dir,
            artifact=Path("reports/static_analysis/polyspace_bug_finder/index.html"),
        )
