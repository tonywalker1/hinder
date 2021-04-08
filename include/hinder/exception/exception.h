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
#include <hinder/misc/compiler.h>
#include <iterator>
#include <stdexcept>
#include <string>

//
// Simplify the definition of new exception types.
//
// Example:
//   HINDER_DEFINE_EXCEPTION(my_error, std::runtime_error);
//
#define HINDER_DEFINE_EXCEPTION(except, except_base)                  \
    struct except : public except_base {                              \
        explicit except(const char* msg) : except_base(msg) {}        \
        explicit except(const std::string& msg) : except_base(msg) {} \
    }

namespace hinder {

    //
    // Define generic_error: the base class for all other exception types used in this library.
    //
    // Example:
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
    // Example:
    //   catch (const std::exception& e) {
    //      std::cout << to_string(e) << '\n';
    //   }
    //
    HINDER_NODISCARD std::string to_string(const std::exception& e, size_t indent = 2);

    //
    // Structured exception message options. Also used by exception_time()
    //
    struct exception_message {
        static std::string time_format;
        static std::string time_key;
        static std::string message_type_key;
        static std::string exception_type_key;
        static std::string message_key;
        static std::string source_key;
    };

    //
    // Get the current time in UTC as a string, see exception_message::time_format.
    //
    HINDER_NODISCARD std::string exception_time() noexcept;

    //
    // Implementation details. May change without warning.
    //
    namespace detail {

#if defined(HINDER_DEFAULT_EXCEPTION_MESSAGE)

        // throw using the default message format
        //   except: message
        template <typename... args>
        HINDER_NODISCARD auto make_exception_message(const char* except,
                                                     const char* fmtstr,
                                                     args&&... a) -> std::string {
            std::string msg;
            fmt::format_to(std::back_inserter(msg), "{}: ", except);
            fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
            return msg;
        }

        // throw using the default message format
        //   except: message @__FILE__:__LINE__
        template <typename... args>
        HINDER_NODISCARD auto make_exception_message(const char* except,
                                                     const char* file,
                                                     int         line,
                                                     const char* fmtstr,
                                                     args&&... a) -> std::string {
            std::string msg;
            fmt::format_to(std::back_inserter(msg), "{}: ", except);
            fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
            fmt::format_to(std::back_inserter(msg), " @{}:{}", file, line);
            return msg;
        }

#elif defined(HINDER_USER_EXCEPTION_MESSAGE)

        // throw using a user defined message format
        //   {0} is always the exception name
        //   {1...n} can be anything you want
        template <typename... args>
        HINDER_NODISCARD auto make_exception_message(const char* except,
                                                     const char* fmtstr,
                                                     args&&... a) -> std::string {
            return fmt::format(fmtstr, except, std::forward<args>(a)...);
        }

        // throw using a user defined message format
        //   {0} is always the exception name
        //   {1} is always __FILE__
        //   {2} is always __LINE___
        //   {3...n} can be anything you want
        template <typename... args>
        HINDER_NODISCARD auto make_exception_message(const char* except,
                                                     const char* file,
                                                     int         line,
                                                     const char* fmtstr,
                                                     args&&... a) -> std::string {
            return fmt::format(fmtstr, except, file, line, std::forward<args>(a)...);
        }

#elif defined(HINDER_STRUCTURED_EXCEPTION_MESSAGE)

        // fill the message string with common JSON data.
        template <typename... args>
        void common_structured_message(std::string& msg,
                                       const char*  except,
                                       const char*  fmtstr,
                                       args&&... a) noexcept {

            fmt::format_to(std::back_inserter(msg),
                           "\"{}\": \"{}\"",
                           exception_message::time_key,
                           exception_time());
            fmt::format_to(std::back_inserter(msg),
                           ", \"{}\": \"exception\"",
                           exception_message::message_type_key);
            fmt::format_to(std::back_inserter(msg),
                           ", \"{}\": \"{}\"",
                           exception_message::exception_type_key,
                           except);
            fmt::format_to(std::back_inserter(msg), ", \"{}\": \"", exception_message::message_key);
            fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
            fmt::format_to(std::back_inserter(msg), "\"");
        }

        // generate the structured message (a JSON object) without file and line
        template <typename... args>
        HINDER_NODISCARD auto make_exception_message(const char* except,
                                                     const char* fmtstr,
                                                     args&&... a) -> std::string {
            std::string msg = "{";
            common_structured_message(msg, except, fmtstr, std::forward<args>(a)...);
            msg += "}";
            return msg;
        }

        // generate the structured message (a JSON object) with file and line
        template <typename... args>
        HINDER_NODISCARD auto make_exception_message(const char* except,
                                                     const char* file,
                                                     int         line,
                                                     const char* fmtstr,
                                                     args&&... a) -> std::string {
            std::string msg = "{";
            common_structured_message(msg, except, fmtstr, std::forward<args>(a)...);
            fmt::format_to(std::back_inserter(msg),
                           ", \"{}\": {{\"file\": \"{}\", \"line\": {}}}}}",
                           exception_message::source_key,
                           file,
                           line);
            return msg;
        }

#endif

    }  // namespace detail
}  // namespace hinder

//
// Throw an exception with a formatted string.
//
// This library uses libfmt (will be included in C++20) to format exception
// messages. See the site for libfmt for documentation.
//
// Parameters:
//   except    The type of exception to throw.
//   var_args  The exception message with arguments.
//
// Example:
//   HINDER_THROW(universe_error, "the answer is {}", 42);
// or
//   HINDER_NESTED_THROW(universe_error, "the answer is {}", 42)
//
#if defined(HINDER_WITH_EXCEPTION_SOURCE)

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
// Example:
//   HINDER_EXPECTS(get_answer() == 42, universe_error, "the answer should be {}", 42);
//

#define HINDER_INVARIANT(cond, except, ...) \
    HINDER_LIKELY(cond) ? ((void)(0)) : HINDER_THROW(except, __VA_ARGS__)

#define HINDER_EXPECTS(cond, except, ...) \
    HINDER_LIKELY(cond) ? ((void)(0)) : HINDER_THROW(except, __VA_ARGS__)

#define HINDER_ENSURES(cond, except, ...) \
    HINDER_LIKELY(cond) ? ((void)(0)) : HINDER_THROW(except, __VA_ARGS__)

#endif  // HINDER_EXCEPTION_H
