//
// hinder::exception
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

#ifndef HINDER_DETAILS_H
#define HINDER_DETAILS_H

#include <fmt/format.h>
#include <hinder/misc/portability.h>
#include <iterator>
#include <string>

#define HINDER_EXCEPTION_NOOP ((void)(0))

namespace hinder {
    namespace detail {

        template <typename... args>
        HINDER_NODISCARD std::string
            make_exception_message(const char* except, const char* fmtstr, args&&... a) {
            std::string msg;
            fmt::format_to(std::back_inserter(msg), "{}: ", except);
            fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
            return msg;
        }

        template <typename... args>
        HINDER_NODISCARD std::string make_exception_message(const char* except,
                                                            const char* file,
                                                            int         line,
                                                            const char* fmtstr,
                                                            args&&... a) {
            std::string msg;
            fmt::format_to(std::back_inserter(msg), "{}: ", except);
            fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
            fmt::format_to(std::back_inserter(msg), " @{}:{}", file, line);
            return msg;
        }

    }  // namespace detail
}  // namespace hinder

#endif  // HINDER_DETAILS_H
