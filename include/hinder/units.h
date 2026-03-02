#pragma once

//
// hinder::misc
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

namespace hinder {

    // Generate size in bytes from size in KiB, MiB, GiB, TiB.
    // For example,
    //     set_buffer(KiB(64));  // yields 64 * 1024 (or 65536) bytes
    constexpr auto KiB(size_t val) noexcept -> size_t {
        const size_t block = 1'024;
        return val * block;  // 2^10
    }

    constexpr auto MiB(size_t val) noexcept -> size_t {
        const size_t block = 1'048'576;
        return val * block;  // 2^20
    }

    constexpr auto GiB(size_t val) noexcept -> size_t {
        const size_t block = 1'073'741'824;
        return val * block;  // 2^30
    }

    constexpr auto TiB(size_t val) noexcept -> size_t {
        const size_t block = 1'099'511'627'776;
        return val * block;  // 2^40
    }

}  // namespace hinder
