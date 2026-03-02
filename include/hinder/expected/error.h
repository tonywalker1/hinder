#pragma once

//
// hinder::error
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

#include <cstddef>
#include <expected>
#include <format>
#include <functional>
#include <hinder/exception/exception_value.h>
#include <map>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace hinder {

    // ========================================================================
    // error: value-type analog of hinder::exception for std::expected
    // ========================================================================

    //
    // Structured error value for use as E in std::expected<T, E>.
    //
    // Mirrors hinder::exception's data model (key-value store, source location,
    // type name) but is a plain value type — copyable, movable, not throwable.
    //
    // Use hinder::fail() to construct via fluent API:
    //   return hinder::fail("command_error")
    //       .message("Failed to execute: {}", command)
    //       .with("exit_code", code);
    //
    class error {
    public:
        using data_map       = std::map<std::string, exception_value, std::less<>>;
        using const_iterator = data_map::const_iterator;

        explicit error(std::string_view     type_name = "error",
                       std::source_location loc       = std::source_location::current());

        // Fluent API — mirrors hinder::exception
        template <typename T>
        auto with(std::string_view key, T && value) -> error & {
            m_data[std::string(key)] = detail::to_exception_value(std::forward<T>(value));
            return *this;
        }

        // Flag-style with (key exists, no value)
        auto with(std::string_view key) -> error &;

        // Convenience for the common "message" key
        template <typename... Args>
        auto message(std::format_string<Args...> fmt, Args &&... args) -> error & {
            m_data["message"] = std::format(fmt, std::forward<Args>(args)...);
            return *this;
        }

        // Accessors — mirrors hinder::exception
        [[nodiscard]] auto get(std::string_view key) const -> std::optional<exception_value>;
        [[nodiscard]] auto contains(std::string_view key) const -> bool;

        template <typename T>
        [[nodiscard]] auto get_as(std::string_view key) const -> std::optional<T>;

        // Iteration over all key-value pairs
        [[nodiscard]] auto begin() const -> const_iterator;
        [[nodiscard]] auto end() const -> const_iterator;
        [[nodiscard]] auto size() const -> std::size_t;

        // Metadata
        [[nodiscard]] auto type_name() const -> std::string_view;
        [[nodiscard]] auto location() const -> std::source_location const &;

    private:
        std::string          m_type_name;
        std::source_location m_location;
        data_map             m_data;
    };

    // ========================================================================
    // error_builder: fluent builder that converts to std::unexpected<error>
    // ========================================================================

    //
    // Returned by hinder::fail(). Proxies error's fluent API and implicitly
    // converts to std::unexpected<error> on return from a function.
    //
    // std::expected<T, error> is constructible from std::unexpected<error>,
    // so this works for any value type T:
    //   std::expected<std::string, error> f() {
    //       return hinder::fail("my_error").message("...");
    //   }
    //
    class error_builder {
    public:
        explicit error_builder(error err) noexcept : m_error(std::move(err)) {}

        template <typename T>
        auto with(std::string_view key, T && value) -> error_builder & {
            m_error.with(key, std::forward<T>(value));
            return *this;
        }

        auto with(std::string_view key) -> error_builder & {
            m_error.with(key);
            return *this;
        }

        template <typename... Args>
        auto message(std::format_string<Args...> fmt, Args &&... args) -> error_builder & {
            m_error.message(fmt, std::forward<Args>(args)...);
            return *this;
        }

        // NOLINTBEGIN(hicpp-explicit-conversions): intentional implicit conversions for fluent API
        // Implicit conversion to std::unexpected<error>
        operator std::unexpected<error>() { return std::unexpected(std::move(m_error)); }

        // Implicit conversion to std::expected<T, error> for any value type T.
        // Avoids requiring two user-defined conversions (error_builder ->
        // std::unexpected -> std::expected), which C++ disallows in implicit sequences.
        template <typename T>
        operator std::expected<T, error>() {
            return std::expected<T, error>(std::unexpect, std::move(m_error));
        }
        // NOLINTEND(hicpp-explicit-conversions)

    private:
        error m_error;
    };

    // ========================================================================
    // Factory and formatting
    // ========================================================================

    //
    // Create an error_builder with fluent API. Source location is captured
    // automatically at the call site via the default parameter.
    //
    // Example:
    //   return hinder::fail("command_error")
    //       .message("Failed to execute: {}", command)
    //       .with("exit_code", code);
    //
    [[nodiscard]] auto fail(std::string_view     type_name = "error",
                            std::source_location loc       = std::source_location::current())
        -> error_builder;

    //
    // Human-readable multi-line format (same layout as to_string(exception const&)).
    //
    // Example output:
    //   command_error @/src/main.cpp:42
    //     message: Failed to execute: ls
    //     exit_code: 1
    //
    [[nodiscard]] auto to_string(error const & err) -> std::string;

    //
    // JSON format for structured logging.
    //
    // Example output:
    //   {"type":"command_error","source":{"file":"/src/main.cpp","line":42},...}
    //
    [[nodiscard]] auto to_json(error const & err) -> std::string;

    // ========================================================================
    // get_as implementation (needs full error definition)
    // ========================================================================

    template <typename T>
    auto error::get_as(std::string_view key) const -> std::optional<T> {
        auto iter = m_data.find(key);
        if (iter == m_data.end()) {
            return std::nullopt;
        }

        return std::visit(
            [](auto const & val) -> std::optional<T> {
                using val_type = std::remove_cvref_t<decltype(val)>;

                // NOLINTNEXTLINE(bugprone-branch-clone)
                if constexpr (std::is_same_v<val_type, std::monostate>) {
                    return std::nullopt;
                } else if constexpr (std::is_same_v<T, val_type>) {
                    return val;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    return value_to_string(val);
                } else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<val_type>) {
                    return static_cast<T>(val);
                } else {
                    return std::nullopt;
                }
            },
            iter->second);
    }

}  // namespace hinder

//
// Return an error with a type name and formatted message in one expression.
//
// Source location is captured automatically. The result is an error_builder,
// so additional context can be chained with .with() when needed.
//
// Parameters:
//   type   String literal naming the error type (e.g., "command_error")
//   fmt    Format string (std::format syntax)
//   ...    Format arguments (optional)
//
// Examples:
//   return HINDER_FAIL("command_error", "Failed to execute: {}", command);
//
//   return HINDER_FAIL("command_error", "Failed to execute: {}", command)
//       .with("exit_code", code);
//
// NOLINTBEGIN(cppcoreguidelines-macro-usage): variadic format wrapper; __VA_OPT__ requires macro
#define HINDER_FAIL(type, fmt, ...) hinder::fail(type).message(fmt __VA_OPT__(, ) __VA_ARGS__)
// NOLINTEND(cppcoreguidelines-macro-usage)

