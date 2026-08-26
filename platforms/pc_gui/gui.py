"""
Drive a variant built for the pc_gui platform from a Python GUI.

The shared library is the same variant every other platform builds, reached through
the same led_interface / button_interface seam the Arduino and TC375 adapters use.
The controls are not hard coded: they are derived from the variant's autoconf.json,
so the window shows exactly the features that variant selected.

    python platforms/pc_gui/gui.py --variant Disco

Build it first with:

    yanga run --variant Disco --platform pc_gui --target build
"""

import argparse
import json
import tkinter as tk
from pathlib import Path
from typing import Optional

from spled_lib import KEY_DOWN, KEY_UP, POWER_BUTTON_KEY, Variant, build_dir, find_library


def read_features(build_dir: Path) -> dict[str, object]:
    features_file = build_dir / "kconfig" / "autoconf.json"
    if not features_file.is_file():
        raise SystemExit(f"Feature configuration not found: {features_file}")
    return json.loads(features_file.read_text())


def knob_labels(features: dict[str, object]) -> Optional[tuple[str, str]]:
    """What the main control knob does in this variant, or None if it has no effect."""
    if features.get("BLINKING"):
        return ("Blink faster", "Blink slower")
    if features.get("BRIGHTNESS_ADJUSTMENT_MANUAL"):
        return ("Brighter", "Dimmer")
    return None


class SpledGui:
    def __init__(self, root: tk.Tk, variant: Variant, variant_name: str, features: dict[str, object]) -> None:
        self.root = root
        self.variant = variant
        self.features = features
        root.title(f"SPLed - {variant_name}")

        self.led = tk.Canvas(root, width=220, height=120, highlightthickness=0, bg="#101010")
        self.led_shape = self.led.create_oval(60, 10, 160, 110, fill="#000000", outline="#303030", width=2)
        self.led.pack(padx=12, pady=12)

        self._add_button("Power", POWER_BUTTON_KEY).pack(fill="x", padx=12)

        labels = knob_labels(features)
        if labels:
            up_label, down_label = labels
            self._add_button(up_label, KEY_UP).pack(fill="x", padx=12, pady=(8, 0))
            self._add_button(down_label, KEY_DOWN).pack(fill="x", padx=12, pady=(4, 0))
        elif features.get("BRIGHTNESS_ADJUSTMENT_AUTOMATIC"):
            period = features.get("BRIGHTNESS_ADJUSTMENT_PERIOD", "?")
            tk.Label(root, text=f"Brightness adjusts automatically every {period} s").pack(padx=12, pady=(8, 0))

        selected = [name for name, value in features.items() if value is True]
        tk.Label(root, text="Selected features: " + ", ".join(selected), wraplength=260, justify="left", fg="#606060").pack(padx=12, pady=12)

        self.tick()

    def _add_button(self, label: str, key_code: int) -> tk.Button:
        """A GUI button is held down like a real one, so the debounce in power_button sees a level rather than an event."""
        button = tk.Button(self.root, text=label)
        button.bind("<ButtonPress-1>", lambda _event: self.variant.set_button(key_code, True))
        button.bind("<ButtonRelease-1>", lambda _event: self.variant.set_button(key_code, False))
        return button

    def tick(self) -> None:
        self.variant.step()
        self.led.itemconfig(self.led_shape, fill=self.variant.led_colour())
        self.root.after(self.variant.task_period_ms, self.tick)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--variant", default="Disco", help="Variant to load.")
    parser.add_argument("--build-type", default="Debug", help="Build type the variant was built with.")
    parser.add_argument("--project-dir", type=Path, default=Path(__file__).resolve().parents[2], help="SPLed project directory.")
    arguments = parser.parse_args()

    directory = build_dir(arguments.project_dir, arguments.variant, arguments.build_type)
    variant = Variant(find_library(directory, arguments.variant))
    features = read_features(directory)

    root = tk.Tk()
    SpledGui(root, variant, arguments.variant, features)
    root.mainloop()


if __name__ == "__main__":
    main()
