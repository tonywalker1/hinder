# hinder::exception

## About

hinder::exception simplifies:

* creating new exceptions,
* testing preconditions, postconditions, and invariants,
* throwing/catching nested exceptions,
* formatting exception messages (using [libfmt](https://github.com/fmtlib/fmt)), and
* generating structured exception messages (also using libfmt).

I have been heavily influenced by years of using Boost Exception. As a result, I tend to use
exceptions as "tags" that identify the source (i.e., file:line) of an exception. The only downside
of Boost Exception is that one has to define lots of exceptions and data types. By using libfmt
(accepted for C++ 20), this library makes it easy to safely format exception messages that hold
useful data without any extra work or types.

Like Boost Exception, this library allows you to collect extra information as the stack unwinds 
toward the ultimate top-level exception handler. However, this library does it via nested 
exceptions. Why nested exceptions? There are a few quick answers most C++ programmers would list,
but here is one that may not be so obvious: because they allow for output that is reminiscent of 
Behavior-driven Development (BDD) and it encourages you to think in those terms when designing 
your error handling strategy. 

This document discusses the to_string() function below, but consider the following possible output 
from a collection of nested exceptions generated via to_string():

```text
file_error: failed to read file "/path/data-2020-01-01T12:34:56.gz" @file:line
  row_error: failed to read line 42 @file:line
    column_error: failed to read column 3 @file:line
      conversion_error: 0xdeadbeef is not a valid number @ file:line
```

From this output, we can see 1) that this specific file has a hexadecimal number that we didn't 
expect, 2) we can see the significant steps leading to the error, and 3) this looks a little 
like a unit test we might write in Catch2 (minus the secnario's, given's, etc.). Take a look at BDD 
syntax in 
[Catch2's documentation](https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md#bdd-style).

## Usage 

### Creating New Exception Types

The base class for all exceptions emitted by this library is **hinder::generic_error**. But unlike,
Boost Exception, QException, etc., you don't need to derive your own exceptions from 
hinder::generic_error. Of course, you may certainly do so (and I encourage you to do so).

To define a new exception, do the following:

```c++
// HINDER_DEFINE_EXCEPTION(<new_type>, <base_class>);
HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
```

### Throw an Exception

To throw an exception, do the following:

```c++
// HINDER_THROW(<exception type>, <exception message>, args);
HINDER_THROW(universe_error, {"the answer is {} not {}", 42, 2});
```

Notice the Python-like formatting. This is thanks to libfmt, see the
[documentation](https://fmt.dev/latest/index.html) of libfmt for details on the formatting
specification.

### Preconditions, Postconditions, and Invariants

Throwing exceptions is great; however, most of the time, you will test some value and THEN throw an 
exception (e.g., test a precondition and then throw). To simplify this, the library provides three 
macros:

```c++
HINDER_EXPECTS(cond, except, ...);     // preconditions
HINDER_ENSURES(cond, except, ...);     // postconditions
HINDER_INVARIANT(cond, except, ...);   // class invariants
```

For example:

```c++
HINDER_EXPECTS(answer == 42, "The answer is 42 not {}", answer);
```

### Nested Exceptions

Like Boost Exception, as the stack unwinds after throwing an exception, you can catch an exception
and add additional information. HOWEVER, unlike Boost exception, you do this by nesting exceptions
rather than modifying the current exception. For example,

```c++
try {
       // some code that throws
}
catch (const std::exception& e) {
    HINDER_NESTED_THROW(another_exception, "some interesting values: {} {} {}",    
                        filename,
                        some_int, 
                        some_double);
}
```

### Output

Eventually, you will want to handle the exception and log the message(s), for example. To output
to stdout, do the following:
```c++
catch (const std::exception& e) {
    std::cerr << hinder::to_string(e) << std::endl;
}
```

to_string() will return the exception message and, in the case of nested exceptions, unwind the
nested exception(s) into a multiline string with indents, for example:

```
last_exception: the last exception message @file:line
  intermediate_exception: the intermediate exception message @file:line
    first_exception: the first exception message @file:line
```

to_string() also takes an optional indent. The default is two spaces.

### Logging 

If you want to write syslog, systemd, or some other logger, this library will provide a better 
solution in the near future.

## Controlling Message Format via CMake Options

The default message format is:
```text
exception_type: exception message @file:line
```

### Controlling File and Line Source

If you do not want __FILE__ and __LINE__ information compiled into your code, you can disable it 
via the HINDER_WITH_EXCEPTION_SOURCE CMake option at comile-time. For example:

```shell
cmake -DHINDER_WITH_EXCEPTION_SOURCE=OFF <path_to_source>
# output will be: exception_type: exception message
```

See [CMake options](./cmake_options.md) for HINDER_WITH_EXCEPTION_SOURCE.

### Changing the Default Format

You may change the message format via the HINDER_EXCEPTION_MESSAGE_FORMAT CMake option. For example:

```shell
cmake -HINDER_EXCEPTION_MESSAGE_FORMAT=User <path_to_source>
```

Setting the **User** option allows you to entirely control the message format. NOTE, you must use 
positional arguments (e.g., {1}) in your format strings. 

Just like argv in main(), the exception name is always {0}. If you enabled source file names and 
line numbers (the default), then the file is {1} and the line numbers is {2}. As an example, you 
could write:

```c++
// with HINDER_WITH_EXCEPTION_SOURCE=ON
HINDER_THROW(my_exception, "The answer is {3} not {4} [{0} at {1},{2}]", 42, 2);
// >> The answer is 42 not 2 [my_exception at @file, line]

// with HINDER_WITH_EXCEPTION_SOURCE=OFF
HINDER_THROW(my_exception, "The answer is {1} not {2} [{0}]", 42, 2);
// >> The answer is 42 not 2 [my_exception]
```

### Structured Logging

Structured logging (JSON formatted messages) can be enabled via the HINDER_EXCEPTION_MESSAGE_FORMAT 
CMake option. For example:

```shell
cmake -HINDER_EXCEPTION_MESSAGE_FORMAT=Structured <path_to_source>
```

Setting the **Structured** formatting option will produce a JSON object similar to the following:

```json
{
  "message time": "2021-04-07T13:10:35.889523258Z",
  "message type": "exception",
  "exception type": "generic_error",
  "message": "The answer is 42 not 2",
  "source": {
    "file": "/usr/local/src/my_project/mycode.cpp",
    "line": 42
  }
}
```

In the above example, I added line breaks to aide readability. The actual output is a single line (
multiline output in logs can be quite problematic). Note: the above output also contains the time of
the message in UTC. I strongly recommend logging in UTC and that is the only option at the moment.
If you want to change the output from the default ISO format, you can do that by changing
exception_message::time_format, see the code for the current string.

When looking at the code for exception_message, you will see a lot of other options you can change.
For example, if you want to use a different key for the exception type, simply change
exception_message::exception_type_key.

```c++
exception_message::exception_type_key = "Exception-Type";
```

### Adding a Timestamp

The library contains a simple convenience function ```std::string exception_time()``` that 
returns the current time as an ISO formatted string (e.g., 2021-04-07T13:10:35.889523258Z). 
While this function is used for structured messages (described above), you can use this function 
to provide a timestamp in your own exception messages.

## Future Directions

Why not use std::format instead of libfmt? At the moment, none of the major compilers implement
std::format, so there is no real choice right now. However, libfmt is well tested and stable, so I
will probably continue to use it for my own work for quite a long time. At some point, std::format
will become widely available and well tested. At that time, I will add a CMake option so you can use
either.
