# hinder

A collection of useful C++ libraries.

To *hinder* something is to make doing that thing more difficult. I chose this name because it is an
antonym of [boost](https://www.boost.org/): my second favorite C++ library. Hopefully, hinder also
boosts your productivity despite the play on words! :wink:

In part this library is the evolution of an earlier attempt to modernize and then share code I have
written over the years. Also, I want to provide some capabilities that I wish were in the standard,
Boost, or other libraries. I would also be ecstatic if you contributed too!

# Documentation and Usage

See the the code for documentation and examples. Reading the code and comments really is the best
way to learn and use this library. I prize clean, simple, readable code. However, I provide general
documentation in the [doc directory](./doc/contents.md).

# Goals

* Easy to use but hard to misuse.
* Flexible but clear, simple, and clean.
* High performance but clear, simple, and clean.
* Container friendly.
* Use the latest C++ standard, techniques, and practices.
* Prefer the standard library, libraries on track for standardization, or Boost.

# Compilers and Platforms

I use Debian Stable for most of my own work. This will be the most tested configuration (at-least by
me). However, in preparation for the release of Bullseye, I am currently using Debian Bullseye
(testing) and am primarily targeting/testing C++17 via GCC 10.2.1.

### C++ Standard

If you do not set CMAKE_CXX_STANDARD, the default is C++ 17. At the moment, the minimum I
test/require is C++11.

### Compilers

* Clang
    * 11.0.1 (Debian Bullseye)
    * 10.0.0 (Ubuntu Focal LTS)
    * 7.0.1 (Debian Buster)
* GCC
    * 10.2.1 (Debian Bullseye)
    * 9.3.0 (Ubuntu Focal LTS)
    * 8.3.0 (Debian Buster)

### Platforms

* Linux
    * Debian
        * Debian 10 (Buster)
        * Debian 11 (Bullseye)
    * Ubuntu 20.04 (Focal; LTS)

See the [container directory](./container) for tools to create containers (via Docker or Podman)
with everything you need to build this library. If you have trouble building this library on your
Linux distribution, let me know by opening an issue. BUT, if you can, try one of the reference
containers. Documentation can be found in the [doc directory](./doc/contents.md).

# Dependencies

See CMakeLists.txt in each subproject for the definitive list dependencies or see the documentation
in the [doc directory](./doc/contents.md).

# Build Options

See the main CMakeLists.txt or the [doc directory](./doc/contents.md) for each subproject.

# Helping

I would love suggestions, fixes, documentation, examples, and other contributions. I would also be
interested in help supporting other platforms. Feel free to discuss additions/contributions with me.

