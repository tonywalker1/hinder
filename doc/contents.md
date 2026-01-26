# hinder Library Documentation

## Building and Installing

* **[CMake Options:](doc/cmake_options.md)** Set various build options.
* **[Dependencies:](./doc/dependencies.md)** Everything you need to build this library.

## Overviews/Tutorials

* Coming soon...

## Reference

* **[assert](./doc/assert.md)** Flexible assert library: formatted messages (via libfmt) and user
  defined assert handlers.

* **[core](./doc/core.md)**
    * **[compiler](doc/compiler.md):** Macros and wrappers to make it easier to support multiple
      compilers and C++ versions.
    * **[format](./doc/format.md):** Define message formatting options.
    * **[timestamp](./doc/timestamp.md):** Produce the current time as an ISO formatted string.
      Perfect for error/log messages.

* **[exception](./doc/exception.md):** Simplifies creating new exceptions, testing preconditions,
  testing postconditions, asserting invariants, throwing/catching exceptions, and formatting error
  messages (using libfmt). Also, supports structured exception messages.

* **misc:** An eclectic collection of useful code.
    * **[units](./doc/units.md):** Useful conversions.
