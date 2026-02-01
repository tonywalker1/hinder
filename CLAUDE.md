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

### Build Directory Layout

The project uses multiple build configurations simultaneously in `build/<kit>-<buildType>` subdirectories (e.g., `build/gcc-debug`, `build/clang-release`). This allows comparing builds, benchmarks, and test results across different compilers and optimization levels without rebuilding.

### CMake Options

- `HINDER_WITH_TESTS` - Build tests (default: ON)
- `HINDER_WITH_EXCEPTION_SOURCE` - Add `__FILE__` and `__LINE__` to exception messages (default: ON)
- `CMAKE_CXX_STANDARD` - C++ standard version (default: 20, minimum: 20)
- `CMAKE_BUILD_TYPE` - Build type (default: Release)

### Running Tests

Tests use Catch2 framework and are automatically discovered via CTest:

```bash
# From build directory - run all tests
ctest

# Run tests with verbose output
ctest -V

# Run specific test executable directly
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

- **core** - Fundamental utilities used by other modules
  - `compiler.h` - Compiler abstraction macros (HINDER_LIKELY, HINDER_NODISCARD, etc.)
  - `platform.h` - Platform detection
  - `format.h` - Message formatting configuration
  - `timestamp.h` - ISO-formatted timestamps for logging
  - Entry: `include/hinder/core/core.h`

- **exception** - Exception utilities with formatted messages and structured output
  - `HINDER_ASSERT()` - Debug-only assertion macro (disabled in release builds)
  - `HINDER_DEFINE_EXCEPTION()` - Macro to define new exception types
  - `HINDER_THROW()` - Throw with formatted message (std::format)
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

**CMake Targets**: The library is built as a single shared library:
- `hinder::hinder` (output: `libhinder.so`)

**Dependencies Between Modules**:
- `core` - No internal dependencies (base module)
- `exception` - Depends on `core`, uses `fmt`
- `misc` - Minimal dependencies (mostly header-only)

### Configurable Utility Pattern

When implementing utilities with optional configuration (like `utc_timestamp()` and
`local_timestamp()`):

- **Provide zero-overhead defaults**: Use static const config objects as default parameters
- **Resolve optional parameters at call time**: For values that might change (like timezone from
  environment), store `nullptr` in config and resolve to actual value in the function body. This
  handles environment changes and avoids static initialization order issues.
- **Thread safety**: No mutable globals. All configuration passes through const parameters.
- **API simplicity**: Default arguments preserve simple `function()` call syntax while allowing
  `function(custom_config)` when needed.

Example: `timestamp.h` config types with `iso_format` static defaults, timezone nullptr resolved to
`current_zone()` at call time.

### External Dependencies

The project uses C++20 standard library features (`<format>` and `<chrono>`) for all formatting and timestamp functionality.

Required libraries (must be available via `find_package`):
- **Catch2** - Testing framework (only when `HINDER_WITH_TESTS=ON`)

### Macro Conventions

The codebase uses several important macros:

- **Assertion**: `HINDER_ASSERT(cond, fmt, ...)` - Debug-only assertion (throws assertion_error)
- **Preconditions**: `HINDER_EXPECTS(cond, exception)` - Always checked
- **Postconditions**: `HINDER_ENSURES(cond, exception)` - Always checked
- **Invariants**: `HINDER_INVARIANT(cond, exception)` - Always checked
- **Exceptions**: `HINDER_THROW(exception)` - Throw with fluent API for formatted message
- **Compiler hints**: `HINDER_LIKELY(x)`, `HINDER_UNLIKELY(x)`, `HINDER_NODISCARD`, `HINDER_NOOP`

All formatting uses std::format syntax: `HINDER_THROW(my_error, "value {} exceeds limit {}", val, max)`

### Testing Structure

Tests mirror the source structure:
- `tests/core/` - Tests for core module
- `tests/exception/` - Tests for exception module (includes HINDER_ASSERT tests)
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
- Use C++20 as minimum standard
- Prefer compile-time checks and modern C++ idioms


When moving a C++ header file from one module/directory, check to see if the header guard name indicates membership to a module/directory and should be similarly edited.

When modifyiing source code with a copyright block at the top, check to see if the copyright year should be updated.
