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

#include <date/date.h>
#include <hinder/exception/exception.h>

namespace hinder {

    std::string exception_message::time_format = "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d}Z";
    std::string exception_message::time_key    = "message time";
    std::string exception_message::message_type_key   = "message type";
    std::string exception_message::exception_type_key = "exception type";
    std::string exception_message::message_key        = "message";
    std::string exception_message::source_key         = "source";


    HINDER_NODISCARD std::string exception_time() noexcept {
        auto t = std::chrono::system_clock::now();

        // break into ymd and time-of-day
        auto dp  = date::floor<date::days>(t);
        auto ymd = date::year_month_day(dp);
        auto tod = date::make_time(t - dp);

        // format the string
        return fmt::format(exception_message::time_format,
                           static_cast<int>(ymd.year()),
                           static_cast<unsigned>(ymd.month()),
                           static_cast<unsigned>(ymd.day()),
                           tod.hours().count(),
                           tod.minutes().count(),
                           tod.seconds().count(),
                           tod.subseconds().count());
    }

    void format_exception(std::string& str, const std::exception& e, size_t indent, size_t level) {
        // format the current exception
        if (level == 0) {
            fmt::format_to(std::back_inserter(str), "{}", e.what());
        } else {
            fmt::format_to(std::back_inserter(str),
                           "\n{}{}",
                           std::string(level * indent, ' '),
                           e.what());
        }

        // unwind any nested exceptions
        try {
            std::rethrow_if_nested(e);
        } catch (std::exception& e) {
            format_exception(str, e, indent, ++level);
        }
    }

    HINDER_NODISCARD std::string to_string(const std::exception& e, size_t indent) {
        std::string msg;
        format_exception(msg, e, indent, 0);
        return msg;
    }

}  // namespace hinder
