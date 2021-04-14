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

#ifndef HINDER_ASSERT_FORMAT_MESSAGE_H
#define HINDER_ASSERT_FORMAT_MESSAGE_H

#include <fmt/format.h>
#include <hinder/assert/assert_config.h>
#include <hinder/core/compiler.h>
#include <hinder/core/format.h>
#include <hinder/core/timestamp.h>
#include <iterator>
#include <stdexcept>
#include <string>

namespace hinder {
    namespace detail {

        template <typename... args>
        HINDER_NODISCARD auto make_assert_message(char const * const cond,
                                                  char const * const file,
                                                  int                line,
                                                  const char * const fmtstr,
                                                  args &&... a) -> std::string {
            std::string msg;
            if (assert_config::format == message_format::DEFAULT) {
                //
                // assert using the default message format
                //   Assertion 'cond' failed: message @__FILE__:__LINE__
                //
                fmt::format_to(std::back_inserter(msg), "Assertion '{}' failed: ", cond);
                fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
                fmt::format_to(std::back_inserter(msg), " @{}:{}", file, line);

            } else if (assert_config::format == message_format::USER) {
                //
                // assert using a user defined message format
                //   {0} is always the condition
                //   {1} is always __FILE__
                //   {2} is always __LINE___
                //   {3...n} can be anything you want
                //
                fmt::format_to(std::back_inserter(msg),
                               fmtstr,
                               cond,
                               file,
                               line,
                               std::forward<args>(a)...);
            } else if (assert_config::format == message_format::STRUCTURED) {
                //
                // generate the structured message (a JSON object) with file and line
                //
                msg += "{";
                fmt::format_to(std::back_inserter(msg),
                               "\"message time\": \"{}\", ",
                               utc_timestamp());
                fmt::format_to(std::back_inserter(msg),
                               "\"message type\": \"assertion failure\", ");
                fmt::format_to(std::back_inserter(msg), "\"assertion condition\": \"{}\", ", cond);

                fmt::format_to(std::back_inserter(msg), "\"message\": \"");
                fmt::format_to(std::back_inserter(msg), fmtstr, std::forward<args>(a)...);
                fmt::format_to(std::back_inserter(msg), "\"");
                fmt::format_to(std::back_inserter(msg),
                               ", \"source\": {{\"file\": \"{}\", \"line\": {}}}",
                               file,
                               line);
                msg += "}";
            }
            return msg;
        }

    }  // namespace detail
}  // namespace hinder

#endif  // HINDER_ASSERT_FORMAT_MESSAGE_H
