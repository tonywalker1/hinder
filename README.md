# hinder

A collection of useful C++ libraries.

To *hinder* something is to make doing that thing more difficult. I chose this name because it is an antonym of *boost*:
my favorite C++ library. Hopefully, hinder also boosts your productivity despite the play on words! ;-)

# Contents

* **exception:** Simplifies creating new exceptions, testing pre-/post-conditions, asserting invariants, throwing/catching
nested exceptions, and formatting error messages (using libfmt).

* **misc:** An eclectic collection of useful code. 
  * **likely:** Yet another implementation for likely and unlikely. 
  * **units:** Useful conversions.

# Goals

* Easy to use but hard to misuse.
* High performance but clear, simple, and clean.
* Use the latest C++ standard, techniques, and practices.
* Prefer the standard library, libraries on track for standardization, or Boost.

# Usage

See the the code for documentation and examples. Reading the code and comments really is the best way to learn and use
this library. I prize clean, simple, readable code. However, I provide some general documentation in the doc directory
or https://github.com/tonywalker1/hinder/tree/unstable/doc.

# Compilers and Platforms

### C++ Standard

If you do not set CMAKE_CXX_STANDARD, the default is C++ 17. At the moment, the minimum is C++ 11.

### Compilers

* Clang
* GCC

### Platforms

* Debian Stable (**What I use.** Also, the reference platform for all development.)

# Dependencies

See each subproject for the definitive list dependencies. However, here is a
(possibly out-of-date) list:

* Catch2 (when testing is enabled; https://github.com/catchorg/Catch2)

# Build Options

See the main CMakeLists.txt or:
```bash
cd <hinder_build_dir>
cmake -L 2>/dev/null | grep HINDER
```

However, here is a (possibly out-of-date) list:

**HINDER_WITH_TESTS** Build tests when ON. [default = ON] 

To use the above options with CMake, do the following from your build directory:
```bash
cmake -D<option>=ON|OFF <path_to_hinder_source> && make -j
```

# Helping

I would love suggestions, fixes, documentation, examples, and other contributions. I would also be interested in help
supporting other platforms. Feel free to discuss additions/contributions with me.
