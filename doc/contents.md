# hinder Library Documentation

## Building and Installing

* **[CMake Options:](doc/cmake_options.md)** Set various build options.
* **[Dependencies:](./doc/dependencies.md)** Everything you need to build this library.

## Overviews/Tutorials

* Coming soon...

## Reference

* **[compiler](doc/compiler.md):** Macros and wrappers to make it easier to support multiple
  compilers and C++ versions (HINDER_LIKELY, HINDER_UNLIKELY, HINDER_NODISCARD, etc.).

* **[exception](./doc/exception.md):** Simplifies creating new exceptions, testing preconditions,
  postconditions, and invariants (HINDER_EXPECTS, HINDER_ENSURES, HINDER_INVARIANT),
  debug assertions (HINDER_ASSERT), throwing/catching exceptions (HINDER_THROW), and formatting error
  messages (using std::format). Also supports structured exception messages.

* **[timestamp](./doc/timestamp.md):** Produce the current time as an ISO formatted string.
  Perfect for error/log messages.

* **[units](./doc/units.md):** Useful conversions (KiB, MiB, GiB, TiB).
