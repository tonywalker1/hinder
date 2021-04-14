//
// hinder::misc
//
// MIT License
//
// Copyright (c) 2021  Tony Walker
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

#ifndef HINDER_MISC_JSON_H
#define HINDER_MISC_JSON_H

#include <core/compiler.h>
#include <fmt/format.h>
#include <type_traits>

namespace hinder {

    template <typename output_t, typename key_t>
    inline auto json_string_to(output_t it, key_t key, const char* value) -> output_t {
        return HINDER_UNLIKELY(value == nullptr) ? fmt::format_to(it, "\"{}\": \"\"", key)
                                                 : fmt::format_to(it, "\"{}\": \"{}\"", key, value);
    }

    template <typename output_t, typename key_t, typename value_t>
    inline auto json_string_to(output_t it, key_t key, value_t value) -> output_t {
        return fmt::format_to(it, "\"{}\": \"{}\"", key, value);
    }

    template <typename output_t, typename key_t, typename value_t>
    inline auto json_number_to(output_t it, key_t key, value_t value) -> output_t {
        return fmt::format_to(it, "\"{}\": {}", key, value);
    }

    template <typename output_t, typename key_t, typename value_t>
    inline auto json_bool_to(output_t it, key_t key, value_t value) -> output_t {
        return value ? fmt::format_to(it, "\"{}\": true", key)
                     : fmt::format_to(it, "\"{}\": false", key);
    }

}  // namespace hinder

#endif  // HINDER_MISC_JSON_H
