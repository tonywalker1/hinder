# hinder::core::timestamp

Produce the current time as an ISO formatted string. Perfect for error/log messages.

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
