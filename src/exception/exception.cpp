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

#include <hinder/exception/exception.h>

namespace hinder {

    exception::exception(std::source_location loc)
        : std::runtime_error("exception")
        , m_location(loc) {}

    auto exception::with(std::string_view key) -> exception& {
        m_data[std::string(key)] = std::monostate{};
        return *this;
    }

    void exception::with_impl(std::string_view key, exception_value val) {
        m_data[std::string(key)] = std::move(val);
    }

    void exception::message_impl(std::string msg) {
        m_data["message"] = std::move(msg);
    }

    auto exception::get(std::string_view key) const -> std::optional<exception_value> {
        auto it = m_data.find(key);
        if (it == m_data.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    auto exception::contains(std::string_view key) const -> bool {
        return m_data.find(key) != m_data.end();
    }

    auto exception::begin() const -> const_iterator {
        return m_data.begin();
    }

    auto exception::end() const -> const_iterator {
        return m_data.end();
    }

    auto exception::size() const -> std::size_t {
        return m_data.size();
    }

    auto exception::type_name() const -> std::string_view {
        return m_type_name;
    }

    auto exception::location() const -> std::source_location const& {
        return m_location;
    }

    auto exception::what() const noexcept -> char const* {
        return m_type_name.c_str();
    }

    void exception::set_type_name(std::string_view name) {
        m_type_name = name;
    }

}  // namespace hinder
