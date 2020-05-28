[![Documentation build](https://readthedocs.org/projects/qutepart-cpp/badge/?version=latest)](https://qutepart-cpp.readthedocs.io/en/latest/)

# Code editor widget for C++/Qt5

Written in pure C++/Qt5. No any dependencies.

Component has been created for [Enki editor](http://enki-editor.org)

Currently there are no API documentation, but [the header](include/qutepart.h) is quite clear.
See also [example application](editor.cpp).

## Features

All features are configurable

* Syntax highlighting for 196 languages
* Smart indentation algorithms:
    * Generic
    * Python
    * Ruby
    * XML
    * Lisp
    * Scheme
    * C, C++
    * JavaScript
    * Java
    * PHP
    * Go
    * And many others
* Line numbers
* Bracket highlighting
* Visible witespaces
* Marker for too long lines
* Bookmarks
* Current line highlighting
* Advanced edit operations:
    - Delete selection or line
    - Duplicate selection or line
    - Move selected lines up or down
    - Copy-paste current or selected lines

## Build dependencies
* C++ compiler
* CMake or qmake
* Qt5 headers

## Building and installation
CMake and qmake is supported. CMake is recommented.

```
mkdir build
cd build
cmake ..
make
make install  # might require sudo
```

## Documentation
To build the docs see [docs](docs/)

Online documetation is available [here](https://qutepart-cpp.readthedocs.io/en/latest/)

## Author
Andrei Kopats

## Bug reports, patches
[Github page](https://github.com/andreikop/qutepart-cpp)

## Qutepart and Katepart
[Kate](http://kate-editor.org/) and Katepart (an editor component) is really cool software. The Kate authors and community have created, probably, the biggest set of highlighters and indenters for programming languages.

* Qutepart uses Kate syntax highlighters (XML files)
* Qutepart contains a port from Javascript to C++ of Kate indenters
* Qutepart doesn't contain Katepart code.

Nothing is wrong with Katepart. Qutepart has been created to enable reusing highlighters and indenters in projects where a KDE dependency is not acceptable.

## License
LGPL v2

I don't mind if you use the code under WTFPL license. But consult you layers if I have right to release it this way.
