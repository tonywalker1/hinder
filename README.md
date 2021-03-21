# hinder

A collection of useful C++ libraries.

To *hinder* something is to make doing that thing more difficult. I
chose this name because it is an antonym of *boost*: my favorite C++ library.
Hopefully, hinder also boosts your productivity despite the play on words! ;-)

# Contents

hinder is organized as a collection of standalone (or minimally interdependent)
libraries that may be cloned and built as needed. See all of the hinder_*
repositories. This may be useful if you only need some of the libraries and do
not want to install all of the necessary dependencies for the libraries you
do not need (e.g., building/running in a container).

HOWEVER, if you do want to build every hinder library, this repository is for
you. The repository will pull (as a git submodule) and build the entire set
of hinder_* libraries.

See the code for a definitive list, but this repository currently pulls and
builds:
* hinder_exception

# Goals

* Use the latest C++ standard, techniques, and practices.
* Prefer the standard library, libraries on track for standardization, or Boost.

# Usage

See the the code for documentation and examples.

# Compilers and Platforms

Each hinder_* library has different requirements: some will compile with
std=c++11 while others require C++17 (for example). See each library for
specific requirements. At a minimum, the following is what I try to support.

### Compilers

* Clang
* GCC

### Platforms

* Linux
    * Debian (stable)
    * Ubuntu (latest)

# Dependencies

See each hinder_* library for specific dependencies.

# Build Options

See each hinder_* library for specific options.

# Helping

I would love suggestions, fixes, documentation, examples, and other
contributions. I would also be interested in help supporting other platforms.
Feel free to discuss additions/contributions with me.
