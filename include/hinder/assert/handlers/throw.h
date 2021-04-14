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

#ifndef HINDER_ASSERT_THROW_H
#define HINDER_ASSERT_THROW_H

#include <hinder/assert/handlers/base.h>
#include <hinder/core/compiler.h>
#include <hinder/exception/exception.h>

namespace hinder {

    //
    // Throws an exception on assert failure. The exception contains msg.
    //
    struct throw_assert_handler : public assert_handler_base {
        virtual void operator()(std::string const & msg) override { throw assertion_error {msg}; }
        virtual auto name() noexcept -> char const * override { return "throw_assert_handler"; };
    };

}  // namespace hinder

#endif  // HINDER_ASSERT_THROW_H
