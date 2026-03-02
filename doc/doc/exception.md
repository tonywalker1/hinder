# hinder::exception

## About

`hinder::exception` simplifies creating exception types, enforcing contracts, and producing
structured error output. The core philosophy — influenced by Boost.Exception — is:

**Store structured data at the throw site. Format at the catch site.**

This keeps error paths readable. Instead of formatting a message string at throw time, you store
typed key-value pairs with the exception and format them when (and how) you actually need them.

The nested exception output is intentionally reminiscent of BDD-style test output:

```text
file_error @/src/mycode.cpp:42
  message: failed to read file "/path/data-2020-01-01T12:34:56.gz"
  row_error @/src/mycode.cpp:87
    message: failed to read line 42
    column_error @/src/mycode.cpp:31
      message: failed to read column 3
      conversion_error @/src/mycode.cpp:15
        message: 0xdeadbeef is not a valid number
```

This encourages thinking in those terms when designing error handling strategy.

## When to Use

See [expected](./expected.md) for the decision between throwing exceptions and returning
`std::expected`. The short version:

| Situation | Tool |
|-----------|------|
| Programming error, contract violation, invariant breach | `throw` + `HINDER_EXPECTS` / `HINDER_ASSERT` |
| Anticipated runtime failure (file not found, parse error) | `std::expected<T, hinder::error>` + `HINDER_FAIL` |

## Usage

### Defining New Exception Types

All user-defined exceptions derive (directly or indirectly) from `hinder::generic_error`, which
itself derives from `hinder::exception` (the library's internal base class). Use the macro:

```c++
HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
```

Exception hierarchies work as expected:

```c++
HINDER_DEFINE_EXCEPTION(io_error, generic_error);
HINDER_DEFINE_EXCEPTION(file_error, io_error);

// file_error is catchable as file_error, io_error, generic_error,
// hinder::exception, std::runtime_error, or std::exception.
```

### Throwing an Exception

`HINDER_THROW` captures source location and returns the exception object for fluent chaining:

```c++
HINDER_THROW(file_error)
    .message("failed to open file")
    .with("path", filename)
    .with("errno", errno);
```

Store any typed value with `.with(key, value)`. Supported types: `bool`, signed/unsigned integers
(stored as `int64_t`/`uint64_t`), `double`, `std::string`, `std::string_view`, `char const*`.

For boolean flags where the presence of the key is itself meaningful:

```c++
HINDER_THROW(network_error)
    .message("connection refused")
    .with("timed_out");  // key present, no value
```

### Contracts: Preconditions, Postconditions, and Invariants

These macros test a condition and throw if it is false. The condition text is automatically
stored as the `"condition"` key; the check type is stored as `"check_type"`.

```c++
HINDER_EXPECTS(ptr != nullptr, null_pointer_error);
// Throws with: condition="ptr != nullptr", check_type="precondition"

HINDER_ENSURES(result >= 0, range_error);
// Throws with: condition="result >= 0", check_type="postcondition"

HINDER_INVARIANT(!list.empty(), state_error);
// Throws with: condition="!list.empty()", check_type="invariant"
```

### Debug Assertions

`HINDER_ASSERT` is only active in debug builds (`NDEBUG` not defined). In release builds it
compiles to nothing.

```c++
HINDER_ASSERT(ptr != nullptr, "pointer must not be null");
HINDER_ASSERT(x > 0, "x={} must be positive", x);
```

Throws `hinder::assertion_error` (derives from `generic_error`) with the condition text,
check type `"assertion"`, and your formatted message.

### Inspecting Exception Data

At the catch site you can read back the stored values:

```c++
catch (hinder::exception const& e) {
    e.type_name();                          // "file_error"
    e.location().file_name();               // "/src/main.cpp"
    e.location().line();                    // 42

    e.contains("path");                     // true/false
    e.get("path");                          // std::optional<exception_value>
    e.get_as<std::string>("path");          // std::optional<std::string>
    e.get_as<int>("errno");                 // std::optional<int>, numeric conversion

    for (auto const& [key, value] : e) {   // iterate all stored pairs
        // ...
    }
}
```

### Output

Two formatting functions are provided:

```c++
catch (hinder::exception const& e) {
    std::cerr << hinder::to_string(e) << '\n';  // human-readable
    std::cerr << hinder::to_json(e) << '\n';    // structured logging
}
```

`to_string` output:

```text
file_error @/src/main.cpp:42
  message: failed to open file
  path: /etc/config.json
  errno: 2
```

`to_json` output (single line; formatted here for readability):

```json
{
  "type": "file_error",
  "source": { "file": "/src/main.cpp", "line": 42 },
  "data": {
    "message": "failed to open file",
    "path": "/etc/config.json",
    "errno": 2
  }
}
```

### Timestamps in Exception Messages

Use the [timestamp](./timestamp.md) module:

```c++
HINDER_THROW(file_error)
    .message("failed to read file at {}", utc_timestamp()());
```

## Design Notes

**Contract macros do not take a format string.** `HINDER_EXPECTS(cond, except)` captures the
condition text automatically via the preprocessor. If you need to include runtime values in the
error, add them with `.with()` after the macro — but note that contract macros throw immediately,
so chaining is not possible. Store context on the exception before the check, or restructure the
check as a manual `if` + `HINDER_THROW`.

**`HINDER_ASSERT` vs. contract macros.** Use `HINDER_ASSERT` for invariants that are only
meaningful in debug builds and would be too expensive (or circular) to check in production.
Use `HINDER_EXPECTS`/`HINDER_ENSURES`/`HINDER_INVARIANT` for checks that must always run.

**CRTP preserves the exception type through fluent chaining.** `HINDER_THROW(my_error).with(...)`
throws `my_error`, not `hinder::exception`. This matters for catch hierarchies.

## Controlling Message Format

The default `to_string` format includes source file and line:

```text
exception_type: message @file:line
```

To omit source location (e.g., for production builds where file paths are sensitive), set the
CMake option at configure time:

```shell
cmake -DHINDER_WITH_EXCEPTION_SOURCE=OFF <path_to_source>
# output: exception_type: message
```

See [CMake options](./cmake_options.md) for details.
