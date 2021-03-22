# hinder::exception

Simplifies:

* creating new exceptions,
* testing preconditions, postconditions, and invariants,
* throwing/catching nested exceptions, and
* formatting error messages (using [libfmt](https://github.com/fmtlib/fmt)).

I have been heavily influenced by years of using Boost Exception. As a result, I tend to use
exceptions as "tags" that identify the source (i.e., file:line) of an exception. The only downside
of Boost Exception is that one has to define lots of exceptions and data types. By using libfmt (
accepted for C++ 20), this library makes it easy to safely format exception messages that hold
useful data without any extra work.

However, for those who like to also see file names and line numbers, this library appends the file
and line to every exception (i.e., "@__FILE__:__LINE__"). If you don't want that, see the CMake
options section below. There are two issues here: 1) compiling the library with file and line
numbers in ONLY applies to code compiled into the .so BUT not to header only code, and 2) compiling
your code with file and line numbers in applies to your code AND the header only code. Puth another
way, compiling your code with file and line off will still cause you to see files and lines if you
compile this library with files and lines on.

Why not use std::format instead of libfmt? As of the time I am writing this, none of the major
compilers implement std::format, so there is no real choice right now. However, libfmt is well
tested and stable, so I will probably continue to use it for my own work for quite a long time. At
some point, std::format will become widely available and well tested. At that time, I will add a
CMake options so you can use either.

# Dependencies

* Catch2 (When testing is enabled; https://github.com/catchorg/Catch2)
* libfmt (Accepted for C++20; https://github.com/fmtlib/fmt)

# CMake Options

* **HINDER_WITH_TESTS** Build tests when ON. [default = ON]
* **HINDER_WITH_EXCEPTION_SOURCE** Append '@__FILE__:__LINE__' to exception message when
  ON [default = ON]"

# Usage

The base class for all exceptions is **hinder::generic_error**. But unlike, Boost Exception,
QException, etc., you don't need to derive your own exceptions from hinder::generic_error. Of course
you can if you want.

To define a new exception, do the following:
```c++
    // HINDER_DEFINE_EXCEPTION(<new_type>, <base_class>);
    HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
```

To throw an exception, do the following:
```c++
    throw random_number_error {"the answer is {}", 42};
```
Notice the Python-like formatting. This is libfmt at work, see the 
[documentation](https://fmt.dev/latest/index.html) for libfmt for details on the formatting 
specification. 

However, most of the time, you will be testing some value and THEN throwing an exception (e.g., 
testing a precondition). To simplify this, the library provides three macros:
```c++
    HINDER_EXPECTS(cond, except, ...)
    HINDER_ENSURES(cond, except, ...)
    HINDER_INVARIANT(cond, except, ...)  
```

Using these is simple, for example:
```c++
    HINDER_EXPECTS(answer == 42, "The answer is 42 not {}", answer);
```

Like Boost Exception, as the stack unwinds after throwing an exception, you can catch an exception 
and add additional information. HOWEVER, unlike Boost exception, you do this by nesting exceptions 
rather than modifying the current exception. For example, 

```c++
   try {
       // some code that throws
   }
   catch (const std::exception& e) {
       HINDER_NESTED_THROW(another_exception, "some interesting values: {} {} {}", filename, 
                           some_int, some_double);
   }
```

Eventually, you will want to capture these exception messages and log them, for example. To do this,
do the following:
```c++
   catch (const std::exception& e) {
       std::cerr << hinder::to_string(e) << std::endl;
   }
```
to_string() will unwind the exceptions into a multiline string with indents, for example:
```
last_exception: the last exception message @file:line
  intermediate_exception: the intermediate exception message @file:line
    first_exception: the first exception message @file:line
```

to_string() also takes an optional indent. The default is two spaces.

That is about it, now look at the code!
