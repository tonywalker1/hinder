# hinder::core::timestamp

Produce the current time as an ISO formatted string. Perfect for error/log messages.

# Usage

For an ISO formatted timestamp in UTC,

```c++
    HINDER_THOW(exception_type, "The error occurred at {}", utc_timestamp());
```

For an ISO formatted timestamp in local time,

```c++
    HINDER_THOW(exception_type, "The error occurred at {}", local_timestamp());
```

If you do not specify a time zone via
```c++
// NOTE: this setting only applies to local_timestamp()
    timestamp_format::time_zone = std::chrono::locate_zone("Europe/Berlin");
```
the function will use your current timezone. This may not be what you want.

While you can use ```local_timestamp()```, I recommend using UTC for all timestamps/logging. It is
the one time which is unambiguous and is not subject to daylight savings times or local legislation
on time zones.

In either case, the default output is of the form:
```text
       UTC:  "2021-04-14T14:41:26.833393854Z"
     Local:  "2021-04-14T10:41:26.833393854Z America/New_York"
```
