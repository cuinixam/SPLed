"""
Load a variant built for the pc_gui platform and drive it from Python.

The shared library is the same variant every other platform builds, reached through
the same led_interface / button_interface seam the Arduino and TC375 adapters use.
Both the GUI (gui.py) and the integration tests under test/ bind to it through here,
so the ABI is described in one place only.

Build a variant first with:

    yanga run --variant Disco --platform pc_gui --target build
"""

import ctypes
from pathlib import Path

PLATFORM = "pc_gui"
LIBRARY_SUFFIXES = (".dll", ".so", ".dylib")

# From components/rte/src/rte_types.h.
KEY_UP = 0x26
KEY_DOWN = 0x28
POWER_BUTTON_KEY = ord("P")


class Variant:
    """The shared library, with the exported ABI bound to Python types."""

    def __init__(self, library_file: Path) -> None:
        self.library = ctypes.CDLL(str(library_file))
        self.library.spled_get_led.argtypes = [ctypes.POINTER(ctypes.c_uint8)] * 3
        self.library.spled_set_button.argtypes = [ctypes.c_int, ctypes.c_int]
        self.library.spled_task_period_ms.restype = ctypes.c_int
        self.library.spled_init()

    @property
    def task_period_ms(self) -> int:
        return self.library.spled_task_period_ms()

    def step(self) -> None:
        self.library.spled_step()

    def led_colour(self) -> str:
        red, green, blue = (ctypes.c_uint8() for _ in range(3))
        self.library.spled_get_led(ctypes.byref(red), ctypes.byref(green), ctypes.byref(blue))
        return f"#{red.value:02x}{green.value:02x}{blue.value:02x}"

    def set_button(self, key_code: int, pressed: bool) -> None:
        self.library.spled_set_button(key_code, 1 if pressed else 0)


def build_dir(project_dir: Path, variant_name: str, build_type: str = "Debug") -> Path:
    return project_dir / ".yanga" / "build" / variant_name / PLATFORM / build_type


def find_library(directory: Path, variant_name: str) -> Path:
    for suffix in LIBRARY_SUFFIXES:
        candidate = directory / f"{variant_name}{suffix}"
        if candidate.is_file():
            return candidate
    raise SystemExit(f"No variant library in {directory}. Build it with: yanga run --variant {variant_name} --platform {PLATFORM} --target build")
