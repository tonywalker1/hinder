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

#include <format>
#include <hinder/exception/exception.h>
#include <iterator>
#include <stdexcept>
#include <string>

namespace hinder {

    void
        format_exception(std::string & str, std::exception const & e, size_t indent, size_t level) {
        // format the current exception
        if (level == 0) {
            std::format_to(std::back_inserter(str), "{}", e.what());
        } else {
            std::format_to(std::back_inserter(str),
                           "\n{}{}",
                           std::string(level * indent, ' '),
                           e.what());
        }

        // unwind any nested exceptions
        try {
            std::rethrow_if_nested(e);
        } catch (std::exception & e) {
            format_exception(str, e, indent, ++level);
        }
    }

    HINDER_NODISCARD std::string to_string(std::exception const & e, size_t indent) {
        std::string msg;
        format_exception(msg, e, indent, 0);
        return msg;
    }

}  // namespace hinder
