# hinder::compiler

Macros and tests to make it easier to support multiple compilers and C++ versions.

# Usage

## C++ Standard Versions
I never remember the version numbers for each C++ standard and have to look them up, so to help with
that, I provide the following macros you can use:

```c++
#define HINDER_CPP_11 201103L;
#define HINDER_CPP_14 201402L;
#define HINDER_CPP_17 201703L;
#define HINDER_CPP_20 202002L;
```

## Likely/Unlikely
Everyone has a version of likely/unlikely macros, so of course I wrote my own. Now, there are n+1 
versions!

```cpp
    if HINDER_LIKELY(answer == 42) {
        // do something interesting
    }
```

When compiled with C++20 support, the above expands to the newly supported [[likely]] and 
[[unlikely]]. Otherwise, the macro uses __builtin_expect() which only works for Clang and GCC.

This macro affects code generation, which you can explore for your own code via 
[Compiler Explorer](https://godbolt.org/). HOWEVER, don't over use it. ONLY use this macro when a 
test is truly (i.e., almost 100%) likely/unlikely. A good example is an invariant or precondition 
test.

## NODISCARD

C++17 added support for [[nodiscard]]. This library provides a macro that expands to [[nodiscard]] 
on C++17 and higher or nothing on C++11 or C++14. Use this macro liberally. It is a good idea and 
can catch simple errors.

```c++
HINDER_NODISCARD int sum(int x, int y);
```

# More?

Yes, there will be more to come...
