# hinder::expected

## About

`hinder::expected` provides structured error values for use with `std::expected<T, E>`. It mirrors
the design of `hinder::exception` — the same key-value store, source location capture, type name,
and fluent API — but as a plain value type rather than a throwable exception.

The core philosophy is the same: **store structured data at the error site, format at the handling
site.** This keeps error paths readable and the data rich without coupling the throw/return site to
any particular output format.

### When to use `hinder::expected` vs. `hinder::exception`

These two modules serve different purposes and are meant to coexist:

| Situation | Tool |
|-----------|------|
| Anticipated runtime failure (file not found, parse error, network timeout) | `std::expected<T, hinder::error>` + `hinder::fail()` |
| Programming error, contract violation, invariant breach | `throw` + `HINDER_EXPECTS` / `HINDER_ASSERT` |

A function returning `std::expected` can still throw for precondition violations — throwing is the
right signal for a bug, while returning an error is the right signal for a recoverable condition.

## Usage

### Returning an Error

The most common pattern uses `HINDER_FAIL`, which captures the type name, formatted message, and
source location in one expression:

```c++
#include <hinder/expected/expected.h>

auto read_file(std::string_view path) -> std::expected<std::string, hinder::error> {
    if (!std::filesystem::exists(path)) {
        return HINDER_FAIL("file_error", "file not found: {}", path);
    }
    // ...
}
```

For errors that carry additional structured data, chain `.with()` calls:

```c++
auto execute(std::string_view command) -> std::expected<std::string, hinder::error> {
    int code = run(command);
    if (code != 0) {
        return HINDER_FAIL("command_error", "failed to execute: {}", command)
            .with("exit_code", code);
    }
    // ...
}
```

### The `hinder::fail()` Factory

`HINDER_FAIL` is shorthand for the common case. For more control, call `hinder::fail()` directly:

```c++
return hinder::fail("parse_error")
    .message("unexpected token '{}' at position {}", token, pos)
    .with("line", line_number)
    .with("column", col_number);
```

Source location is captured automatically at the `fail()` call site via `std::source_location`.

### Flag-Style Keys

For boolean conditions where the presence of the key is itself meaningful:

```c++
return hinder::fail("network_error", "connection refused")
    .with("timed_out");   // key present, no value
```

### Handling Errors

At the call site, use the standard `std::expected` interface:

```c++
auto result = read_file("/etc/config.json");
if (!result) {
    std::cerr << hinder::to_string(result.error()) << '\n';
    return;
}
use(*result);
```

Or with the monadic interface for composing operations:

```c++
auto result = read_file(path)
    .and_then(parse_json)
    .transform_error([](hinder::error const& err) {
        // wrap or enrich the error
        return err;
    });
```

### Inspecting Error Data

`hinder::error` exposes the same accessor interface as `hinder::exception`:

```c++
auto const& err = result.error();

err.type_name();                        // "command_error"
err.location().file_name();             // "/src/main.cpp"
err.location().line();                  // 42

err.contains("exit_code");              // true/false
err.get("exit_code");                   // std::optional<exception_value>
err.get_as<int>("exit_code");           // std::optional<int> with numeric conversion
err.get_as<std::string>("exit_code");   // converts any value to string

for (auto const& [key, value] : err) { // iterate all key-value pairs
    // ...
}
```

### Output

Two formatting functions mirror those provided by `hinder::exception`:

```c++
hinder::to_string(err);   // human-readable, for logs and stderr
hinder::to_json(err);     // JSON object, for structured logging
```

`to_string` output:

```text
command_error @/src/main.cpp:42
  message: failed to execute: ls /nonexistent
  exit_code: 2
```

`to_json` output (single line, formatted here for readability):

```json
{
  "type": "command_error",
  "source": { "file": "/src/main.cpp", "line": 42 },
  "data": {
    "message": "failed to execute: ls /nonexistent",
    "exit_code": 2
  }
}
```

## Design Notes

**`to_json` omits `"data"` when the error has no key-value pairs.** An error constructed with
only a type name produces `{"type":"...","source":{...}}` — no `"data"` field. This keeps the
JSON compact for simple error signals.

**Source location is captured at the `fail()` call site, not at the `error` constructor.**
The second parameter of `hinder::fail()` is a `std::source_location` default parameter — never
pass it explicitly. If you construct `hinder::error` directly, source location is captured at
the `error(...)` call site.
