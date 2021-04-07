# hinder

A collection of useful C++ libraries.

To *hinder* something is to make doing that thing more difficult. I chose this name because it is an
antonym of [boost](https://www.boost.org/): my **second** favorite C++ library. Hopefully, hinder
also boosts your productivity despite the play on words! :wink:

In part this library is the evolution of an earlier attempt to modernize and then share code I have
written over the years. Also, I want to provide some capabilities that I wish were in the C++
standard, Boost, or other libraries. I would also be ecstatic if you contributed too!

## Goals

* Easy to use but hard to misuse.
* Flexible but clear, simple, and clean.
* High performance but clear, simple, and clean.
* Container friendly.
* Do as much at compile-time as possible.
* Use the latest C++ standard, techniques, and practices.
* Prefer the standard library, libraries on track for standardization, or Boost.

## Documentation and Usage

Reading the code and comments really is a great way to learn and use this library. I prize clean,
simple, readable code. I provide general documentation in the [doc directory](./doc/contents.md).

## Compilers and Platforms

### C++ Standard

If you do not set CMAKE_CXX_STANDARD, the default is C++17. At the moment, the minimum supported
standard is C++11.

### Compilers

* Clang
* GCC

### Platforms

* Linux

See the [container directory](./container) for tools to create reference containers (via Docker or
Podman) with everything you need to build this library. If you have trouble building this library on
your Linux distribution, let me know by opening an issue. BUT, if you can, try one of the reference
containers.

## Dependencies

See the [doc directory](./doc/contents.md) for details.

## Build Options

See the [doc directory](./doc/contents.md) for details.

## Installing
```shell
mkdir -p <path>/build
git clone https://github.com/tonywalker1/hinder <path>
cd <path>/build
cmake <options> ..
make -j
make install
```

## Contributing

I would love suggestions, fixes, documentation, examples, and other contributions. I would also be
interested in help supporting other platforms. See [CONTRIBUTING](./CONTRIBUTING.md) and 
[CODE_OF_CONDUCT](./CODE_OF_CONDUCT.md) for details.

See [CONTRIBUTORS](./CONTRIBUTORS.md) for a list of contributors.
