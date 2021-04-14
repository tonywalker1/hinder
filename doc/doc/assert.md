# hinder::assert

A flexible assert library that supports formatted messages 
(via [libfmt](https://github.com/fmtlib/fmt)) and user defined assert handlers.

## Usage

### Asserting a Condition

Using the library is much like C assert's except you can provide a message. For example,

```c++
int answer = 2;
HINDER_ASSERT(answer == 42, "The answer should be {} not {}", 42, answer);
// --> "Assertion 'answer == 42' failed: The answer should be 42 not 2 @file:line"
```

Notice the Python-like formatting. This is thanks to libfmt, see the
[libfmt documentation](https://fmt.dev/latest/index.html) of libfmt for details on the formatting
specification.

### Formatting the Messages

You may change the message format via ```exception_config::format```. For example,
```c++
#include <hinder/exception/exception.h>
int main()
{
    hinder::exception_config::format = message_format::DEFAULT;
    ...
}
```
Currently, the options are DEFAULT, USER, or STRUCTRUED.

Setting the **USER** option allows you to entirely control the message format. NOTE, you must use
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

Setting the **STRUCTURED** formatting option will produce a JSON object similar to the following:

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
timestamp_format::utc_format, see the code for the current string.

