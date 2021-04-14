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

#ifndef HINDER_ASSERT_CLASSIC_H
#define HINDER_ASSERT_CLASSIC_H

#include <cstdlib>
#include <hinder/assert/handlers/base.h>
#include <iostream>
#include <string>
#include <stdexcept>

namespace hinder {

    //
    // Classic assert handler (i.e., writes a message to stderr and aborts).
    //
    // This handler defaults to std::cerr, but you can pass any stream to the constructor. Of
    // course, the stream needs to remain open for the duration of the program.
    //
    class classic_assert_handler : public assert_handler_base {
    public:
        classic_assert_handler(std::ostream & out = std::cerr) : m_out {out} {}

        virtual void operator()(std::string const & msg) override {
            m_out << msg << std::endl;
            std::terminate();
        }

        virtual auto name() noexcept -> char const * override { return "classic_assert_handler"; };

    private:
        std::ostream & m_out;
    };

}  // namespace hinder

#endif  // HINDER_ASSERT_CLASSIC_H
