//
// hinder::error formatting
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

#include <format>
#include <hinder/expected/error.h>
#include <iterator>
#include <string>

namespace hinder {

    namespace {

        auto escape_json_string(std::string_view input) -> std::string {
            std::string result;
            result.reserve(input.size());
            for (char c : input) {
                switch (c) {
                case '"':
                    result += "\\\"";
                    break;
                case '\\':
                    result += "\\\\";
                    break;
                case '\n':
                    result += "\\n";
                    break;
                case '\r':
                    result += "\\r";
                    break;
                case '\t':
                    result += "\\t";
                    break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        std::format_to(std::back_inserter(result),
                                       "\\u{:04x}",
                                       static_cast<unsigned>(c));
                    } else {
                        result += c;
                    }
                }
            }
            return result;
        }

        auto value_to_json(exception_value const& val) -> std::string {
            return std::visit([](auto const& v) -> std::string {
                using T = std::remove_cvref_t<decltype(v)>;

                if constexpr (std::is_same_v<T, std::monostate>) {
                    return "null";
                } else if constexpr (std::is_same_v<T, bool>) {
                    return v ? "true" : "false";
                } else if constexpr (std::is_same_v<T, std::string>) {
                    return std::format("\"{}\"", escape_json_string(v));
                } else {
                    return std::format("{}", v);
                }
            }, val);
        }

    }  // namespace

    auto to_string(error const& err) -> std::string {
        std::string result;

        // Header: type @file:line
        auto const& loc = err.location();
        std::format_to(std::back_inserter(result),
                       "{} @{}:{}",
                       err.type_name(),
                       loc.file_name(),
                       loc.line());

        // Key-value pairs, indented
        for (auto const& [key, value] : err) {
            auto val_str = value_to_string(value);
            if (val_str.empty()) {
                std::format_to(std::back_inserter(result), "\n  {}", key);
            } else {
                std::format_to(std::back_inserter(result), "\n  {}: {}", key, val_str);
            }
        }

        return result;
    }

    auto to_json(error const& err) -> std::string {
        std::string result;
        result += '{';

        // Type
        std::format_to(std::back_inserter(result),
                       "\"type\":\"{}\"",
                       err.type_name());

        // Source location
        auto const& loc = err.location();
        std::format_to(std::back_inserter(result),
                       ",\"source\":{{\"file\":\"{}\",\"line\":{}}}",
                       escape_json_string(loc.file_name()),
                       loc.line());

        // Data
        if (err.size() > 0) {
            result += ",\"data\":{";
            bool first = true;
            for (auto const& [key, value] : err) {
                if (!first) {
                    result += ',';
                }
                first = false;
                std::format_to(std::back_inserter(result),
                               "\"{}\":{}",
                               escape_json_string(key),
                               value_to_json(value));
            }
            result += '}';
        }

        result += '}';
        return result;
    }

}  // namespace hinder
