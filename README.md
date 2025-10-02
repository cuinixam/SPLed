# SPLED: Software Product Line Engineering Demo

This demo repository contains a tiny but fully fledged [SPL](https://en.wikipedia.org/wiki/Software_product_line) for SW development. It is capable of holding a set of software modules that are differently used and configured among the variants. The build environment is based on [YANGA](https://github.com/cuinixam/yanga).

[![CI](https://github.com/cuinixam/spled/actions/workflows/test.yml/badge.svg)](https://github.com/cuinixam/spled/actions/workflows/test.yml)

## Start developing

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
