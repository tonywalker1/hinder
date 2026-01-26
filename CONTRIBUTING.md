# Contributing to hinder

Thank you for your interest in contributing! This document outlines the development workflow and
contribution guidelines.

## Code of Conduct

This project adheres to a [Code of Conduct](./CODE_OF_CONDUCT.md). By participating, you are
expected to uphold this code.

## Development Workflow

### Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR-USERNAME/hinder.git`
3. Add upstream remote: `git remote add upstream https://github.com/tonywalker1/hinder.git`

### Making Changes

1. Create a feature branch from `main`: `git checkout -b feature-name`
2. Make your changes following the code standards below
3. Build and test your changes:
   ```bash
   mkdir -p build && cd build
   cmake -DHINDER_WITH_TESTS=ON ..
   make -j
   ctest -V
   ```
4. Commit your changes with a clear commit message (imperative mood: "Add feature", not "Added feature")

### Submitting Changes

1. Push your branch to your fork: `git push origin feature-name`
2. Open a Pull Request against the `main` branch
3. Describe what your changes do and why
4. Link any related issues

### After Submission

- Respond to review feedback
- Make requested changes by pushing new commits to your branch
- Once approved, a maintainer will merge your PR

## Code Standards

### C++ Standard

This project requires **C++20** as the minimum standard.

### Code Style

- Follow the `.clang-format` configuration: `clang-format -i <file>`
- Pass `.clang-tidy` checks: `clang-tidy <file> -- -std=c++20 -I./include`
- Use modern C++ idioms and prefer compile-time checks
- Write clear, simple, readable code

### Code Organization

- **Headers**: `include/hinder/<module>/`
- **Implementation**: `src/<module>/`
- **Tests**: `tests/<module>/` using Catch2 framework
- **Documentation**: `doc/doc/<module>.md`

### Testing

- Add tests for all new functionality
- Ensure existing tests pass before submitting
- Run specific test suites: `./tests/<module>/hinder_<module>_tests`

## What to Contribute

We welcome:

- **Bug fixes** - Include a test that demonstrates the bug if possible
- **New utilities** - Explain the use case and why it fits hinder's goals
- **Documentation improvements** - Typos, clarifications, examples
- **Platform support** - Help testing on different Linux distributions
- **Performance improvements** - Include benchmarks demonstrating the improvement

## Questions?

Open an issue for discussion before starting major changes. This helps avoid duplicate work and
ensures your contribution aligns with the project's direction.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
