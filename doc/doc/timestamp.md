# hinder::core::timestamp

Produce the current time as an ISO formatted string. Perfect for error/log messages.

Both `utc_timestamp()` and `local_timestamp()` accept an optional `std::chrono::system_clock::time_point` parameter for testing purposes, but default to using the current time when called without arguments.

# Usage

For an ISO formatted timestamp in UTC,

```c++
    HINDER_THROW(exception_type, "The error occurred at {}", utc_timestamp());
```

For an ISO formatted timestamp in local time,

```c++
    HINDER_THROW(exception_type, "The error occurred at {}", local_timestamp());
```

By default, `local_timestamp()` uses your current timezone. To specify a different timezone or format:

```c++
    auto berlin_config = local_timestamp_config{
        "{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d} {}",  // Custom format
        std::chrono::locate_zone("Europe/Berlin")
    };
    HINDER_THROW(exception_type, "The error occurred at {}", local_timestamp(berlin_config));
```

You can also customize the UTC timestamp format:

```c++
    auto custom_utc = utc_timestamp_config{"{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}.{:09d}Z"};
    auto ts = utc_timestamp(custom_utc);
```

While you can use ```local_timestamp()```, I recommend using UTC for all timestamps/logging. It is
the one time which is unambiguous and is not subject to daylight savings times or local legislation
on time zones.

In either case, the default output is of the form:
```text
       UTC:  "2021-04-14T14:41:26.833393854Z"
     Local:  "2021-04-14T10:41:26.833393854Z America/New_York"
```

# Testing

For deterministic testing, both functions accept an optional `std::chrono::system_clock::time_point`:

```c++
    using namespace std::chrono;

    // Create a specific time: 2021-04-14 14:41:26.833393854 UTC
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s + 833'393'854ns;

    auto result = utc_timestamp(utc_timestamp_config::iso_format, tp);
    // result == "2021-04-14T14:41:26.833393854Z"
```

This allows tests to verify exact timestamp formatting without depending on the current system time.
