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

#ifndef HINDER_EXCEPTION_DETAIL_H
#define HINDER_EXCEPTION_DETAIL_H

#include <fmt/format.h>
#include <hinder/core/compiler.h>
#include <hinder/core/format.h>
#include <hinder/core/timestamp.h>
#include <hinder/exception/exception_config.h>
#include <iterator>
#include <utility>

namespace hinder {
    namespace detail {

        template <typename... args>
        [[nodiscard]] auto make_exception_message(char const * const except,
                                                  char const * const fmtstr,
                                                  args &&... a) -> std::string {
            std::string msg;
            switch (exception_config::format) {
            case message_format::DEFAULT:
                //
                // throw using the default message format
                //   except: message
                //
                fmt::format_to(std::back_inserter(msg), "{}: ", except);
                fmt::format_to(std::back_inserter(msg),
                               fmt::runtime(fmtstr),
                               std::forward<args>(a)...);
                break;
            case message_format::USER:
                //
                // throw using a user defined message format
                //   {0} is always the exception name
                //   {1...n} can be anything you want
                //
                return fmt::format(fmt::runtime(fmtstr), except, std::forward<args>(a)...);
                break;
            case message_format::STRUCTURED:
                //
                // generate the structured message (a JSON object) with file and line
                //
                msg += "{";
                fmt::format_to(std::back_inserter(msg),
                               "\"message time\": \"{}\", ",
                               utc_timestamp());
                fmt::format_to(std::back_inserter(msg), "\"message type\": \"exception\", ");
                fmt::format_to(std::back_inserter(msg), "\"exception type\": \"{}\", ", except);

                fmt::format_to(std::back_inserter(msg), "\"message\": \"");
                fmt::format_to(std::back_inserter(msg),
                               fmt::runtime(fmtstr),
                               std::forward<args>(a)...);
                fmt::format_to(std::back_inserter(msg), "\"");

                msg += "}";
                break;
            }
            return msg;
        }

        template <typename... args>
        [[nodiscard]] auto make_exception_message(char const * const except,
                                                  char const * const file,
                                                  int                line,
                                                  char const * const fmtstr,
                                                  args &&... a) noexcept -> std::string {
            std::string msg;
            switch (exception_config::format) {
            case message_format::DEFAULT:
                //
                // throw using the default message format
                //   except: message @__FILE__:__LINE__
                //
                fmt::format_to(std::back_inserter(msg), "{}: ", except);
                fmt::format_to(std::back_inserter(msg),
                               fmt::runtime(fmtstr),
                               std::forward<args>(a)...);
                fmt::format_to(std::back_inserter(msg), " @{}:{}", file, line);
                break;
            case message_format::USER:
                // throw using a user defined message format
                //   {0} is always the exception name
                //   {1} is always __FILE__
                //   {2} is always __LINE___
                //   {3...n} can be anything you want
                return fmt::format(fmt::runtime(fmtstr),
                                   except,
                                   file,
                                   line,
                                   std::forward<args>(a)...);
                break;
            case message_format::STRUCTURED:
                //
                // generate the structured message (a JSON object) without file and line
                //
                msg += "{";
                fmt::format_to(std::back_inserter(msg),
                               "\"message time\": \"{}\", ",
                               utc_timestamp());
                fmt::format_to(std::back_inserter(msg), "\"message type\": \"exception\", ");
                fmt::format_to(std::back_inserter(msg), "\"exception type\": \"{}\", ", except);
                fmt::format_to(std::back_inserter(msg), "\"message\": \"");
                fmt::format_to(std::back_inserter(msg),
                               fmt::runtime(fmtstr),
                               std::forward<args>(a)...);
                fmt::format_to(std::back_inserter(msg), "\"");
                fmt::format_to(std::back_inserter(msg),
                               ", \"source\": {{\"file\": \"{}\", \"line\": {}}}",
                               file,
                               line);
                msg += "}";
                break;
            }
            return msg;
        }

    }  // namespace detail
}  // namespace hinder

#endif  // HINDER_EXCEPTION_DETAIL_H
