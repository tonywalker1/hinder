# hinder::core::timestamp

Produce the current time as an ISO formatted string. Perfect for error/log messages.

`utc_ts` and `local_ts` are callable objects (functors) that capture their format string and
timezone at construction and produce strings on demand. The free functions `utc_timestamp()` and
`local_timestamp()` return references to default-configured instances.

# Usage

For an ISO formatted timestamp in UTC,

```c++
    HINDER_THROW(exception_type).message("The error occurred at {}", utc_timestamp()());
```

For an ISO formatted timestamp in local time,

```c++
    HINDER_THROW(exception_type).message("The error occurred at {}", local_timestamp()());
```

By default, `local_timestamp()` uses your current timezone. To specify a different timezone or
format, construct a `local_ts` instance directly:

```c++
    auto berlin = local_ts{
        "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d} {}",
        std::chrono::locate_zone("Europe/Berlin")
    };
    HINDER_THROW(exception_type, "The error occurred at {}", berlin());
```

You can also customize the UTC timestamp format:

```c++
    auto custom_utc = utc_ts{"{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}.{:09d}Z"};
    auto ts = custom_utc();
```

### Custom Format Argument Order

The format string receives positional arguments in this order:

| Position | `utc_ts` | `local_ts` |
|----------|----------|------------|
| 0 | year | year |
| 1 | month | month |
| 2 | day | day |
| 3 | hour | hour |
| 4 | minute | minute |
| 5 | second | second |
| 6 | nanoseconds | nanoseconds |
| 7 | *(none)* | timezone name |

A format string for `local_ts` must include all 8 slots (or use positional syntax to skip one).
Providing fewer than 8 slots means the last slots receive unexpected arguments.

While you can use `local_timestamp()`, I recommend using UTC for all timestamps/logging. It is
the one time which is unambiguous and is not subject to daylight savings times or local legislation
on time zones.

In either case, the default output is of the form:
```text
       UTC:  "2021-04-14T14:41:26.833393854Z"
     Local:  "2021-04-14T10:41:26.833393854 America/New_York"
```

# Testing

For deterministic testing, `operator()` accepts an optional `std::chrono::system_clock::time_point`:

```c++
    using namespace std::chrono;

    // Create a specific time: 2021-04-14 14:41:26.833393854 UTC
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s + 833'393'854ns;

    auto result = utc_ts{}(tp);
    // result == "2021-04-14T14:41:26.833393854Z"
```

This allows tests to verify exact timestamp formatting without depending on the current system time.
