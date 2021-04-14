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

#ifndef HINDER_ASSERT_H
#define HINDER_ASSERT_H

#include <hinder/assert/assert_config.h>
#include <hinder/assert/detail/format_message.h>
#include <hinder/assert/handlers/base.h>
#include <hinder/core/compiler.h>

//
// Assert that a condition is true. If not, a formatted message (via libfmt) is passed to the
// installed assertion handler. See set_assert_handler() on details for changing the assertion
// handler.
//
// Example:
//     HINDER_ASSERT(answer == 42, "Your answer of {} is wrong! {} is correct.", answer, 42);
//
#ifdef NDEBUG
    #define HINDER_ASSERT(cond, ...) ((void)(0))
#else
    #define HINDER_ASSERT(cond, ...)                  \
        HINDER_LIKELY(cond)                           \
        ? HINDER_NOOP                                 \
        : hinder::assert_config::handler->operator()( \
            hinder::detail::make_assert_message((#cond), __FILE__, __LINE__, __VA_ARGS__))
#endif

#endif  // HINDER_ASSERT_H
