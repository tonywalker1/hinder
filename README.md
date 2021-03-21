# hinder

A collection of useful C++ libraries.

To *hinder* something is to make doing that thing more difficult. I chose this name because it is an
antonym of [boost](https://www.boost.org/): my favorite C++ library. Hopefully, hinder also boosts
your productivity despite the play on words! :wink:

# Documentation and Usage

See the the code for documentation and examples. Reading the code and comments really is the best
way to learn and use this library. I prize clean, simple, readable code. However, there is general
documentation in the [doc directory](./doc/contents.md).

# Goals

* Easy to use but hard to misuse.
* High performance but clear, simple, and clean.
* Use the latest C++ standard, techniques, and practices.
* Prefer the standard library, libraries on track for standardization, or Boost.

# Compilers and Platforms

I currently use C++17 on Debian 11 (Bullseye) and GCC 10.2 for most of my own work. This will be the
most tested configuration. At-least by me. When Bullseye is released and becomes stable, this is
what I will use until use until right before the next release (usually shortly before the first
freeze).

### C++ Standard

If you do not set CMAKE_CXX_STANDARD, the default is C++ 17. At the moment, the minimum I
test/require is C++11.

### Compilers

* Clang
* GCC

### Platforms

* Debian Stable (The reference platform for all development.)

See the [docker directory](./docker) for tools to build containers with everything you need to
build this library. 

# Dependencies

See CMakeLists.txt in each subproject for the definitive list dependencies or see the documentation
in the [doc directory](./doc/contents.md).

# Build Options

See the main CMakeLists.txt or the [doc directory](./doc/contents.md) for each subproject. However,
there is one global option:

* **HINDER_WITH_TESTS** Build tests when ON. [default = ON]

To use the option(s) with CMake, do the following from your build directory:

```bash
cd <path_to_hinder_build_dir>
cmake -D<option>=ON|OFF <path_to_hinder_source> && make -j && make install
```

# Helping

I would love suggestions, fixes, documentation, examples, and other contributions. I would also be
interested in help supporting other platforms. Feel free to discuss additions/contributions with me.

