//
// hinder::assert
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

#ifndef HINDER_ASSERT_BASE_H
#define HINDER_ASSERT_BASE_H

#include <memory>
#include <string>

namespace hinder {

    //
    // Base class for all assert handlers.
    //
    // * operator() is called by HINDER_ASSERT() to handle a failure of the assertion condition.
    // * msg is a libfmt formatted string (see libfmt for documentation on the format
    // specification).
    // * name() returns a human-readable name for the object in case you want to log which handler
    // is installed
    //
    struct assert_handler_base : std::enable_shared_from_this<assert_handler_base> {
        virtual void operator()(std::string const & msg) = 0;
        virtual auto name() noexcept -> char const *     = 0;
    };

}  // namespace hinder

#endif  // HINDER_ASSERT_BASE_H
