//
// hinder::exception
//
// MIT License
//
// Copyright (c) 2019-2026  Tony Walker
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

#include <cstddef>
#include <format>
#include <hinder/compiler.h>
#include <hinder/exception/exception_value.h>
#include <map>
#include <optional>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace hinder {

    // ========================================================================
    // Implementation details
    // ========================================================================

    namespace detail {

        // Type trait to detect string-like types
        // Uses std::decay_t which converts arrays to pointers
        template<typename T>
        struct is_string_like : std::false_type {};

        template<>
        struct is_string_like<std::string> : std::true_type {};

        template<>
        struct is_string_like<std::string_view> : std::true_type {};

        template<>
        struct is_string_like<char const*> : std::true_type {};

        template<>
        struct is_string_like<char*> : std::true_type {};

        template<typename T>
        inline constexpr bool is_string_like_v = is_string_like<std::decay_t<T>>::value;

        // Helper to convert arbitrary types to exception_value
        template<typename T>
        auto to_exception_value(T&& value) -> exception_value {
            using decayed = std::decay_t<T>;

            if constexpr (std::is_same_v<decayed, bool>) {
                return value;
            } else if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::monostate>) {
                return value;
            } else if constexpr (is_string_like_v<T>) {
                return std::string(std::forward<T>(value));
            } else if constexpr (std::is_floating_point_v<decayed>) {
                return static_cast<double>(value);
            } else if constexpr (std::is_signed_v<decayed> && std::is_integral_v<decayed>) {
                return static_cast<std::int64_t>(value);
            } else if constexpr (std::is_unsigned_v<decayed> && std::is_integral_v<decayed>) {
                return static_cast<std::uint64_t>(value);
            } else {
                static_assert(std::is_same_v<decayed, void>,
                    "Unsupported type for exception::with(). "
                    "Supported types: bool, integers, floating point, strings.");
            }
        }

    }  // namespace detail

    // ========================================================================
    // Base exception class
    // ========================================================================

    //
    // Base exception class with key-value data storage.
    //
    // Design philosophy:
    //   - Store structured data at throw site
    //   - Format at catch site
    //   - Fluent API for adding context (using CRTP for derived types)
    //
    // Example:
    //   throw file_error()
    //       .message("Failed to open file")
    //       .with("path", filename)
    //       .with("errno", errno);
    //
    class exception : public std::runtime_error {
    public:
        using data_map = std::map<std::string, exception_value, std::less<>>;
        using const_iterator = data_map::const_iterator;

        explicit exception(std::source_location loc = std::source_location::current());

        // Fluent API for adding data (returns exception& for base class)
        template<typename T>
        auto with(std::string_view key, T&& value) -> exception& {
            m_data[std::string(key)] = detail::to_exception_value(std::forward<T>(value));
            return *this;
        }

        // Flag-style with (key exists, no value)
        auto with(std::string_view key) -> exception&;

        // Convenience for the common "message" key
        template<typename... Args>
        auto message(std::format_string<Args...> fmt, Args&&... args) -> exception& {
            m_data["message"] = std::format(fmt, std::forward<Args>(args)...);
            return *this;
        }

        // Accessors
        [[nodiscard]] auto get(std::string_view key) const -> std::optional<exception_value>;
        [[nodiscard]] auto contains(std::string_view key) const -> bool;

        // Typed access with conversion
        template<typename T>
        [[nodiscard]] auto get_as(std::string_view key) const -> std::optional<T>;

        // Iteration over all key-value pairs
        [[nodiscard]] auto begin() const -> const_iterator;
        [[nodiscard]] auto end() const -> const_iterator;
        [[nodiscard]] auto size() const -> std::size_t;

        // Metadata
        [[nodiscard]] auto type_name() const -> std::string_view;
        [[nodiscard]] auto location() const -> std::source_location const&;

        // std::exception interface
        [[nodiscard]] auto what() const noexcept -> char const* override;

    protected:
        void set_type_name(std::string_view name);

        // Protected implementation methods for CRTP derived classes
        void with_impl(std::string_view key, exception_value val);
        void message_impl(std::string msg);

    private:
        std::string m_type_name{"exception"};
        std::source_location m_location;
        data_map m_data;
    };

    // ========================================================================
    // CRTP mixin for fluent interface with proper return types
    // ========================================================================

    //
    // CRTP base class that provides fluent methods returning the derived type.
    // This preserves the exception type when chaining: throw my_error().with(...) throws my_error.
    //
    // Template parameters:
    //   Derived - The concrete exception class (for return type)
    //   Base    - The parent exception class to inherit from
    //
    template<typename Derived, typename Base>
    class exception_crtp : public Base {
    public:
        explicit exception_crtp(std::source_location loc = std::source_location::current())
            : Base(loc) {}

        // Fluent API returning Derived& to preserve type through chaining
        template<typename T>
        auto with(std::string_view key, T&& value) -> Derived& {
            Base::with_impl(key, detail::to_exception_value(std::forward<T>(value)));
            return static_cast<Derived&>(*this);
        }

        // Flag-style with (key exists, no value)
        auto with(std::string_view key) -> Derived& {
            Base::with_impl(key, std::monostate{});
            return static_cast<Derived&>(*this);
        }

        // Convenience for the common "message" key
        template<typename... Args>
        auto message(std::format_string<Args...> fmt, Args&&... args) -> Derived& {
            Base::message_impl(std::format(fmt, std::forward<Args>(args)...));
            return static_cast<Derived&>(*this);
        }
    };

    // ========================================================================
    // get_as implementation (needs full exception definition)
    // ========================================================================

    template<typename T>
    auto exception::get_as(std::string_view key) const -> std::optional<T> {
        auto it = m_data.find(key);
        if (it == m_data.end()) {
            return std::nullopt;
        }

        return std::visit([](auto const& val) -> std::optional<T> {
            using val_type = std::remove_cvref_t<decltype(val)>;

            if constexpr (std::is_same_v<val_type, std::monostate>) {
                return std::nullopt;
            } else if constexpr (std::is_same_v<T, val_type>) {
                return val;
            } else if constexpr (std::is_same_v<T, std::string>) {
                // Convert anything to string
                return value_to_string(val);
            } else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<val_type>) {
                // Allow numeric conversions
                return static_cast<T>(val);
            } else {
                return std::nullopt;
            }
        }, it->second);
    }

}  // namespace hinder

//
// Simplify the definition of new exception types.
//
// Uses CRTP to ensure fluent methods return the correct derived type,
// preserving the exception type when chaining on throw statements.
//
// Example:
//   HINDER_DEFINE_EXCEPTION(my_error, hinder::generic_error);
//
//   throw my_error()
//       .message("Something failed")
//       .with("code", 42);  // Still throws my_error, not exception
//
#define HINDER_DEFINE_EXCEPTION(name, base)                                             \
    class name : public hinder::exception_crtp<name, base> {                            \
    public:                                                                             \
        explicit name(std::source_location loc = std::source_location::current())       \
            : hinder::exception_crtp<name, base>(loc) {                                 \
            set_type_name(#name);                                                       \
        }                                                                               \
    }

namespace hinder {

    //
    // Define generic_error: the base class for all other exception types used in this library.
    //
    // Example:
    //   HINDER_DEFINE_EXCEPTION(random_number_error, generic_error);
    //
    HINDER_DEFINE_EXCEPTION(generic_error, exception);

    //
    // Used by the assert module when using throw_assert_handler, see hinder/assert/handlers/throw.h.
    //
    HINDER_DEFINE_EXCEPTION(assertion_error, generic_error);

}  // namespace hinder

//
// Throw an exception with fluent API.
//
// Returns the exception object for chaining with .message() and .with().
//
// Example:
//   HINDER_THROW(file_error)
//       .message("Failed to open file")
//       .with("path", filename);
//
#define HINDER_THROW(except) \
    throw except(std::source_location::current())

//
// Contract checking macros.
//
// These macros auto-capture the condition text as the "condition" key.
// They also set "check_type" to identify the contract type.
//
// Parameters:
//   cond    Condition to test, throw on false.
//   except  The type of exception to throw if cond is false.
//
// Example:
//   HINDER_EXPECTS(ptr != nullptr, null_pointer_error);
//   // Throws with: condition="ptr != nullptr", check_type="precondition"
//

#define HINDER_EXPECTS(cond, except) \
    HINDER_LIKELY(cond) ? HINDER_NOOP : \
        throw except(std::source_location::current()) \
            .with("condition", #cond) \
            .with("check_type", "precondition")

#define HINDER_ENSURES(cond, except) \
    HINDER_LIKELY(cond) ? HINDER_NOOP : \
        throw except(std::source_location::current()) \
            .with("condition", #cond) \
            .with("check_type", "postcondition")

#define HINDER_INVARIANT(cond, except) \
    HINDER_LIKELY(cond) ? HINDER_NOOP : \
        throw except(std::source_location::current()) \
            .with("condition", #cond) \
            .with("check_type", "invariant")

//
// Debug-only assertion macro.
//
// Unlike contract macros (EXPECTS/ENSURES/INVARIANT), HINDER_ASSERT is only checked
// in debug builds (when NDEBUG is not defined). In release builds, it compiles to
// a no-op with zero overhead.
//
// Parameters:
//   cond    Condition to test, throw assertion_error on false.
//   fmt     Format string (std::format syntax).
//   ...     Format arguments (optional).
//
// Examples:
//   HINDER_ASSERT(ptr != nullptr, "pointer must not be null");
//   HINDER_ASSERT(x > 0, "x={} must be positive", x);
//
#ifdef NDEBUG
    #define HINDER_ASSERT(cond, ...) ((void)(0))
#else
    #define HINDER_ASSERT(cond, ...) \
        HINDER_LIKELY(cond) ? HINDER_NOOP : \
            throw ::hinder::assertion_error(std::source_location::current()) \
                .with("condition", #cond) \
                .with("check_type", "assertion") \
                .message(__VA_ARGS__)
#endif

namespace hinder {

    //
    // Human-readable multi-line format.
    //
    // Example output:
    //   file_error @/src/main.cpp:42
    //     message: Failed to open file
    //     path: /etc/config.json
    //     errno: 2
    //
    [[nodiscard]] auto to_string(exception const& exc) -> std::string;

    //
    // JSON format for structured logging.
    //
    // Example output:
    //   {"type":"file_error","source":{"file":"/src/main.cpp","line":42},...}
    //
    [[nodiscard]] auto to_json(exception const& exc) -> std::string;

}  // namespace hinder

#endif  // HINDER_EXCEPTION_H
