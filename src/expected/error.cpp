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

#include <hinder/exception/exception_value.h>
#include <hinder/expected/error.h>

#include <cstddef>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <variant>

namespace hinder {

    error::error(std::string_view type_name, std::source_location loc)
    : m_type_name(type_name),
      m_location(loc) {}

    auto error::with(std::string_view key) -> error & {
        m_data[std::string(key)] = std::monostate {};
        return *this;
    }

    auto error::get(std::string_view key) const -> std::optional<exception_value> {
        auto iter = m_data.find(key);
        if (iter == m_data.end()) {
            return std::nullopt;
        }
        return iter->second;
    }

    auto error::contains(std::string_view key) const -> bool {
        return m_data.find(key) != m_data.end();
    }

    auto error::begin() const -> const_iterator { return m_data.begin(); }

    auto error::end() const -> const_iterator { return m_data.end(); }

    auto error::size() const -> std::size_t { return m_data.size(); }

    auto error::type_name() const -> std::string_view { return m_type_name; }

    auto error::location() const -> std::source_location const & { return m_location; }

    auto fail(std::string_view type_name, std::source_location loc) -> error_builder {
        return error_builder(error(type_name, loc));
    }

}  // namespace hinder
