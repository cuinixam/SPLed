# Shared library and Python

`pc_gui` builds the variant as a shared library (`Disco.dll`, `.so` or `.dylib`) with a
small C API on top of the scheduler: init, one task step, the buttons in, the LED out.
`platforms/pc_gui/spled_lib.py` binds that API with ctypes, and both the GUI and the
integration tests under `test/` go through it.

## Build and run the GUI

```bash
yanga run --variant Disco --platform pc_gui --target build
python platforms/pc_gui/gui.py --variant Disco
```

The window's controls come from the variant's `autoconf.json`, so it shows exactly the
features that variant selected. `--build-type` and `--project-dir` are the other options.

## Explore from Python

`pyproject.toml` puts `platforms/pc_gui` on pytest's `pythonpath`; from a REPL add it
yourself. Everything is synchronous: the library only advances when you call `step()`,
so a scenario is a plain sequence of calls.

```python
import sys
from pathlib import Path

sys.path.insert(0, "platforms/pc_gui")
from spled_lib import KEY_DOWN, KEY_UP, POWER_BUTTON_KEY, Variant, build_dir, find_library

project = Path.cwd()
variant = Variant(find_library(build_dir(project, "Disco"), "Disco"))

variant.task_period_ms       # ms of product time per step()
variant.set_button(POWER_BUTTON_KEY, True)
for _ in range(10):          # power_button debounces over ten steps
    variant.step()
variant.led_colour()         # "#008000": green at the default brightness
variant.power_state          # 1 = ON
variant.set_button(POWER_BUTTON_KEY, False)

variant.set_button(KEY_UP, True)   # main control knob: blink rate (Disco), brightness (Sleep, Spa)
for _ in range(10):
    variant.step()
variant.main_knob_value, variant.brightness
```

The button stays pressed until you release it, like a finger on the board. To watch a
blink, step a few hundred times and collect `led_colour()` after each.

## From exploration to a test

`test/test_Disco.py` is the pattern: a module fixture builds the library through
`RunCommand` and returns a `Variant`, and a test drives it exactly as above with asserts
in place of a look. Copy that fixture into the variant's test file and keep the scenario
as the sequence you typed.
