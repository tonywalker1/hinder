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

#ifndef HINDER_EXCEPTION_VALUE_H
#define HINDER_EXCEPTION_VALUE_H

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace hinder {

    //
    // Supported value types for exception key-value storage.
    // Covers the most common use cases while keeping the type system simple.
    //
    using exception_value = std::variant<
        std::monostate,      // key exists, no value (flag)
        bool,
        std::int64_t,
        std::uint64_t,
        double,
        std::string
    >;

    //
    // Convert an exception_value to a string representation.
    //
    [[nodiscard]] auto value_to_string(exception_value const& v) -> std::string;

    namespace detail {

        // Type trait to detect string-like types.
        // Uses std::decay_t which converts arrays to pointers.
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

        // Convert arbitrary types to exception_value.
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
                    "Unsupported type for with(). "
                    "Supported types: bool, integers, floating point, strings.");
            }
        }

    }  // namespace detail

}  // namespace hinder

#endif  // HINDER_EXCEPTION_VALUE_H
