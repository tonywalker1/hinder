# C++ Style Guidelines

Rules for `*.h` and `*.cpp` files in this project.

## Core Principles

- **Const by default**: Mark everything `const` unless mutation is required
- **No side effects**: Prefer pure functions; isolate side effects at boundaries
- **No undefined behavior**: Never rely on UB, even if "it works on my machine"
- **Small functions**: Each function does one thing and is independently testable

## Contracts

Document preconditions, postconditions, and invariants:

- Use `HINDER_EXPECTS(cond, exception, fmt, ...)` for preconditions
- Use `HINDER_ENSURES(cond, exception, fmt, ...)` for postconditions
- Use `HINDER_INVARIANT(cond, exception, fmt, ...)` for class/loop invariants
- Use `HINDER_ASSERT(cond, fmt, ...)` for debug-only checks
- Add brief comments for contracts that aren't self-evident from the condition

## Functional Style

- Prefer returning values over output parameters
- Prefer `std::optional` or `std::expected` for "might not have a value" over sentinel values
- Prefer algorithms and ranges over raw loops where clarity is maintained
- Avoid mutable state; pass configuration through const parameters
