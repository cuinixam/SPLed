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

    @pytest.mark.build
    def test_build(self):
        super().test_build(build_type="Debug")

    @pytest.mark.unittests
    def test_unittests(self):
        super().test_unittests(build_type="Debug")

    @pytest.mark.reports
    def test_reports(self):
        super().test_reports(build_type="Debug")
