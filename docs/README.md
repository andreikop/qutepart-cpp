## Qutepart documentation

Qutepart API shall be well documented.

This directory contains scripts, configuration files and documentation sources.

The documentation is generated in two stages:

1. Doxygen extracts in-code documentation from the headers.
2. Sphinx build developer-readable docs based on data extracted by Doxygen and hand-written high level descriptions contained in .rst files in this directory.

To build docs run from the top-level directory

```
    mkdir build
    cd build
    cmake ..
    make sphinx-docs
```

The documentation is saved in `build/docs/sphinx`.
