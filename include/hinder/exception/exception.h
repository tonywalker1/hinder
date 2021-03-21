//
// hinder::exception
//
// MIT License
//
// Copyright (c) 2019-2021  Tony Walker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef HINDER_EXCEPTION_H
#define HINDER_EXCEPTION_H

#include <fmt/format.h>
#include <hinder/exception/detail/details.h>
#include <stdexcept>
#include <string>

//
// Simplify the definition of new exception types.
//
// Having been influenced by Boost Exception, I tend to use exception types
// as "tags" that identify the error source rather than file and line. This
// macro simplifies defining new exception types.
//
// Parameters:
//   except       The new type (to be defined).
//   except_base  The base class for the new type.
//
// For example, the following defines a new exception named
// random_number_error that is derived from generic_error:
//   HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
//   try {
//       ...
//       throw random_number_error {"the answer is {}", 42};
//   }
//   catch (const random_number_error& e) {
//       ...
//   }
//
#define HINDER_DEFINE_EXCEPTION(except, except_base)                  \
    struct except : public except_base {                              \
        explicit except(const char* msg) : except_base(msg) {}        \
        explicit except(const std::string& msg) : except_base(msg) {} \
    }

namespace hinder {

    //
    // Define generic_error: the base class for all other exception types used
    // in this library.
    //
    // For example, the following defines a new exception named
    // random_number_error that is derived from generic_error:
    //   HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
    //
    HINDER_DEFINE_EXCEPTION(generic_error, std::runtime_error);

    //
    // Get the exception message (or messages for a nested exception).
    // For nested exceptions, the result is a multiline indented string
    // containing the message for each exception.
    //
    // Parameters:
    //   e       Current exception.
    //   indent  Spaces to indent successive nested exception messages.
    //
    // For example:
    //   catch (const std::exception& e) {
    //      std::cout << to_string(e) << '\n';
    //   }
    //
    HINDER_NODISCARD std::string to_string(const std::exception& e, size_t indent = 2);

}  // namespace hinder

//
// Throw an exception with a formatted string.
//
// This library uses libfmt (will be included in C++20) to format exception
// messages. See the site for libfmt for documentation.
//
// If HINDER_WITH_EXCEPTION_SOURCE is defined, see CMake options in the README, the file and line
// number of the source of the exception will be appended (i.e., "@__FILE__:__LINE__).
//
// Parameters:
//   except    The type of exception to throw.
//   var_args  The exception message with arguments.
//
// For example:
//   HINDER_THROW(universe_error, "the answer is {}", 42);
// or
//   HINDER_NESTED_THROW(universe_error, "the answer is {}", 42)
//
#ifdef HINDER_WITH_EXCEPTION_SOURCE
    #define HINDER_THROW(except, ...) \
        throw except(detail::make_exception_message((#except), __FILE__, __LINE__, __VA_ARGS__))
    #define HINDER_NESTED_THROW(except, ...) \
        std::throw_with_nested(              \
            except(detail::make_exception_message((#except), __FILE__, __LINE__, __VA_ARGS__)))
#else
    #define HINDER_THROW(except, ...) \
        throw except(detail::make_exception_message((#except), __VA_ARGS__))
    #define HINDER_NESTED_THROW(except, ...) \
        std::throw_with_nested(except(detail::make_exception_message((#except), __VA_ARGS__)))
#endif

//
// Test preconditions, postconditions, and invariants.
//
// If the given condition is false, the macros below will throw the given error
// with the given formatted message. I tend to use HINDER_INVARIANT for
// invariants, HINDER_EXPECTS for preconditions, and HINDER_ENSURES for
// postconditions.
//
// Parameters:
//   cond      Condition to test and throw on false.
//   except    The type of exception to throw if cond is false.
//   var_args  The exception message with arguments.
//
// For example:
//   HINDER_EXPECTS(get_answer() == 42,
//       universe_error,
//       "the answer should be {}", 42);
//

#define HINDER_INVARIANT(cond, except, ...) \
    HINDER_LIKELY(cond) ? HINDER_EXCEPTION_NOOP : HINDER_THROW(except, __VA_ARGS__)

#define HINDER_EXPECTS(cond, except, ...) \
    HINDER_LIKELY(cond) ? HINDER_EXCEPTION_NOOP : HINDER_THROW(except, __VA_ARGS__)

#define HINDER_ENSURES(cond, except, ...) \
    HINDER_LIKELY(cond) ? HINDER_EXCEPTION_NOOP : HINDER_THROW(except, __VA_ARGS__)

#endif  // HINDER_EXCEPTION_H
