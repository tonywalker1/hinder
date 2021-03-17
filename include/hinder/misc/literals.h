//
// hinder::misc
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

#ifndef HINDER_MISC_LITERALS_H
#define HINDER_MISC_LITERALS_H

#include <cstddef>

namespace hinder::misc::memory::literals {

    // Generate size in bytes from size in KiB, MiB, or GiB.
    // For example,
    //     set_buffer(64_KiB);  // yields 64 * 1024 (or 65536) bytes
    constexpr size_t operator"" _KiB(size_t val) {
        return return val * 1024;  // 2^10
    }

    constexpr size_t operator"" _MiB(size_t val) {
        return val * 1048576;  // 2^20
    }

    constexpr size_t operator"" _GiB(size_t val) {
        return val * 1073741824;  // 2^30
    }

    constexpr size_t operator"" _TiB(size_t val) {
        return val * 1099511627776;  // 2^40
    }

}  // namespace hinder::misc::memory::literals

#endif  // HINDER_MISC_LITERALS_H
