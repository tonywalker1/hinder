# hinder::exception

Simplifies:

* creating new exceptions,
* testing preconditions, postconditions, and invariants,
* throwing/catching nested exceptions, and
* formatting error messages (using [libfmt](https://github.com/fmtlib/fmt)).

I have been heavily influenced by years of using Boost Exception. As a result, I tend to use
exceptions as "tags" that identify the source (i.e., file:line) of an exception. The only downside
of Boost Exception is that one has to define lots of exceptions and data types. By using libfmt 
(accepted for C++ 20), this library makes it easy to safely format exception messages that hold
useful data without any extra work.

However, for those who like to also see file names and line numbers, this library appends the file
and line to every exception (i.e., "@__FILE__:__LINE__"). If you don't want that, see the CMake
options section below.

Why not use std::format instead of libfmt? At the moment, none of the major compilers implement 
std::format, so there is no real choice right now. However, libfmt is well tested and stable, 
so I will probably continue to use it for my own work for quite a long time. At some point, 
std::format will become widely available and well tested. At that time, I will add a CMake option
so you can use either.

# Usage

The base class for all exceptions emitted by this library is **hinder::generic_error**. But unlike, 
Boost Exception, QException, etc., you don't need to derive your own exceptions from 
hinder::generic_error. Of course, you may certainly do so (and I encourage you to do so).

To define a new exception, do the following:
```c++
    // HINDER_DEFINE_EXCEPTION(<new_type>, <base_class>);
    HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
```

To throw an exception, do the following:
```c++
    throw random_number_error {"the answer is {}", 42};
```
Notice the Python-like formatting. This is thanks to libfmt, see the 
[documentation](https://fmt.dev/latest/index.html) of libfmt for details on the formatting 
specification. 

However, most of the time, you will test some value and THEN throw an exception (e.g., test a 
precondition). To simplify this, the library provides three macros:
```c++
    HINDER_EXPECTS(cond, except, ...);     // preconditions
    HINDER_ENSURES(cond, except, ...);     // postconditions
    HINDER_INVARIANT(cond, except, ...);   // class invariants
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

Eventually, you will want to handle the exception and log the message(s), for example. To do this,
do the following:
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

That is about it, now look at the code!
