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

}  // namespace hinder

#endif  // HINDER_EXCEPTION_VALUE_H
