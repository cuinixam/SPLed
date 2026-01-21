# SPLED: Software Product Line Engineering Demo

This demo repository contains a tiny but fully fledged [SPL](https://en.wikipedia.org/wiki/Software_product_line) for SW development. It is capable of holding a set of software modules that are differently used and configured among the variants. The build environment is based on [YANGA](https://github.com/cuinixam/yanga).

[![CI](https://github.com/cuinixam/spled/actions/workflows/test.yml/badge.svg)](https://github.com/cuinixam/spled/actions/workflows/test.yml)

## Start developing

### Windows (PowerShell)

For installing the dependencies run:

```powershell
.\build.ps1 -install
```

To build a variant from the command line run:

```powershell
.\yanga.ps1 run
```

It is recommended to start VS Code using the `build.ps1` script to make sure all dependencies and environment variables are properly configured.

```powershell
.\build.ps1 -startVSCode
```

### Linux/Unix (Bash)

Install C/C++ build tools (Ubuntu/Debian):

```bash
sudo apt-get update
sudo apt-get install -y gcc g++ clang cmake ninja-build cppcheck
```

Install pypeline runner and create virtual environment:

```bash
pip install pipx
pipx install pypeline-runner
pypeline run --config-file yanga.yaml --step CreateVEnv --single
```

Activate the virtual environemt and run `yanga`:

```bash
source .venv/bin/activate
yanga run
```

### Devcontainer

The devcontainer has all dependencies pre-installed (including yanga). Simply run:

```bash
yanga run
```

## Credits

The original implementation of this demo using [SPL Core](https://github.com/avengineers/spl-core) CMake modules can be found [here](https://github.com/avengineers/SPLed).
