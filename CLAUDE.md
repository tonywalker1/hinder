# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**hinder** is a collection of C++ utility libraries with a focus on compile-time safety, clear error
messages, and modern C++ practices. The name is a play on "boost" (the library's second favorite
after the C++ standard library).

## Build System

This project uses CMake with a modular structure where each library component can be built and
installed independently.

### Building the Project

```bash
# Standard build from repository root
mkdir -p build
cd build
cmake ..
make -j
make install
```

### CMake Options

- `HINDER_WITH_TESTS` - Build tests (default: ON)
- `HINDER_WITH_EXCEPTION_SOURCE` - Add `__FILE__` and `__LINE__` to exception messages (default: ON)
- `CMAKE_CXX_STANDARD` - C++ standard version (default: 17, minimum: 11)
- `CMAKE_BUILD_TYPE` - Build type (default: Release)

### Running Tests

Tests use Catch2 framework and are automatically discovered via CTest:

```bash
# From build directory - run all tests
ctest

# Run tests with verbose output
ctest -V

# Run specific test executable directly
./tests/assert/hinder_assert_tests
./tests/exception/hinder_exception_tests
./tests/misc/hinder_misc_tests
```

### Code Quality Tools

```bash
# clang-format is configured in .clang-format (use for formatting)
clang-format -i <file>

# clang-tidy is configured in .clang-tidy (extensive checks enabled)
clang-tidy <file> -- -std=c++17 -I./include
```

## Architecture

### Library Structure

The project is organized into independent modules under `src/` and `include/hinder/`:

- **assert** - Flexible assertion library with customizable handlers and formatted messages
  - Supports pluggable assertion handlers (classic abort, throw exception, or custom)
  - Uses libfmt for message formatting
  - Controlled by NDEBUG (disabled in release builds)
  - Entry: `include/hinder/assert/assert.h`

- **core** - Fundamental utilities used by other modules
  - `compiler.h` - Compiler abstraction macros (HINDER_LIKELY, HINDER_NODISCARD, etc.)
  - `platform.h` - Platform detection
  - `format.h` - Message formatting configuration
  - `timestamp.h` - ISO-formatted timestamps for logging
  - Entry: `include/hinder/core/core.h`

- **exception** - Exception utilities with formatted messages and structured output
  - `HINDER_DEFINE_EXCEPTION()` - Macro to define new exception types
  - `HINDER_THROW()` - Throw with formatted message (libfmt)
  - `HINDER_EXPECTS/ENSURES/INVARIANT()` - Contract checking macros
  - Support for nested exceptions with `to_string()`
  - Entry: `include/hinder/exception/exception.h`

- **misc** - Miscellaneous utilities (units conversions, JSON strings, etc.)
  - Entry: `include/hinder/misc/` (header-only utilities)

### Key Design Patterns

**Namespace**: All code lives in `namespace hinder`. Internal implementation details are in
`namespace hinder::detail`.

**Header Organization**: Each module has a convenience header that includes all subcomponents
(e.g., `hinder/core/core.h` includes all core headers).

**CMake Targets**: Each library is built as a shared library with an alias:
- `hinder::assert` (output: `libhinder_assert.so`)
- `hinder::core` (output: `libhinder_core.so`)
- `hinder::exception` (output: `libhinder_exception.so`)
- `hinder::misc` (output: `libhinder_misc.so`)

**Dependencies Between Modules**:
- `core` - No internal dependencies (base module)
- `assert` - Depends on `core`, uses `fmt` and `date-tz`
- `exception` - Depends on `core`, uses `fmt`
- `misc` - Minimal dependencies (mostly header-only)

### External Dependencies

Required libraries (must be available via `find_package`):
- **fmt** - String formatting library (used extensively)
- **date** - Howard Hinnant's date library (used by assert/core for timestamps)
- **Catch2** - Testing framework (only when `HINDER_WITH_TESTS=ON`)

### Macro Conventions

The codebase uses several important macros:

- **Assertion**: `HINDER_ASSERT(cond, fmt, ...)` - Checked only in debug builds
- **Preconditions**: `HINDER_EXPECTS(cond, exception, fmt, ...)` - Always checked
- **Postconditions**: `HINDER_ENSURES(cond, exception, fmt, ...)` - Always checked
- **Invariants**: `HINDER_INVARIANT(cond, exception, fmt, ...)` - Always checked
- **Exceptions**: `HINDER_THROW(exception, fmt, ...)` - Throw with formatted message
- **Compiler hints**: `HINDER_LIKELY(x)`, `HINDER_UNLIKELY(x)`, `HINDER_NODISCARD`, `HINDER_NOOP`

All formatting uses libfmt syntax: `HINDER_THROW(my_error, "value {} exceeds limit {}", val, max)`

### Testing Structure

Tests mirror the source structure:
- `tests/assert/` - Tests for assert module
- `tests/core/` - Tests for core module (currently commented out in root CMakeLists.txt)
- `tests/exception/` - Tests for exception module
- `tests/misc/` - Tests for misc module

Each test directory has:
- `main.cpp` - Catch2 test runner setup
- `*_tests.cpp` - Actual test cases
- `CMakeLists.txt` - Uses `catch_discover_tests()` for CTest integration

## Development Workflow

### Adding New Functionality

When adding features to an existing module:
1. Add header to `include/hinder/<module>/`
2. Add implementation (if not header-only) to `src/<module>/`
3. Update `src/<module>/CMakeLists.txt` if adding new .cpp files
4. Add tests to `tests/<module>/`
5. Update module documentation in `doc/doc/<module>.md` (if it exists)

### Creating New Module

The project structure expects each module to:
1. Have headers in `include/hinder/<module>/`
2. Have source in `src/<module>/` with CMakeLists.txt
3. Have tests in `tests/<module>/` with CMakeLists.txt
4. Export CMake config in `cmake/hinder_<module>Config.cmake`
5. Be added to root CMakeLists.txt via `add_subdirectory()`

### Code Style

The project enforces strict code quality:
- Follow `.clang-format` configuration (appears to be LLVM-based style)
- Pass `.clang-tidy` checks (extensive checks enabled: boost, bugprone, cert, cppcoreguidelines,
  hicpp, modernize, performance, readability, etc.)
- Use C++11 as minimum, C++17 as default
- Prefer compile-time checks and modern C++ idioms

## Container Support

Reference containers are provided in `container/` for reproducible builds:
- `debian-buster/` - Debian 10 environment
- `debian-bullseye/` - Debian 11 environment
- `ubuntu-focal/` - Ubuntu 20.04 LTS environment

Build and run containers using:
```bash
./container/build_container.sh <platform>
./container/run_container.sh <platform>
```
