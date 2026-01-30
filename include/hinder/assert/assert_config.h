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

#ifndef HINDER_ASSERT_CONFIG_H
#define HINDER_ASSERT_CONFIG_H

#include <hinder/assert/handlers/base.h>
#include <hinder/core/format.h>
#include <memory>

namespace hinder {

    //
    // Configure assert handlers and output.
    //
    struct assert_config {

        //
        // Assertion handler: called when the asserted condition is false.
        //
        // Several handlers are available, see include/hinder/assert/handlers, or you can define
        // your own. By deriving from assert_handler_base, see
        // include/hinder/assert/handlers/base.h.
        //
        // To change the handler,
        //     hinder::assert_config::handler = std::make_unique<throw_assert_handler>();
        //
        // The default handler is "classic_assert_handler".
        //
        static std::unique_ptr<assert_handler_base> handler;

        //
        // Set the message format, see include/hinder/core/format.h.
        //
        // The options are currently DEFAULT, USER, and STRUCTURED. THe default is DEFAULT. ;-)
        //
        static message_format format;
    };

}  // namespace hinder

#endif  // HINDER_ASSERT_CONFIG_H
