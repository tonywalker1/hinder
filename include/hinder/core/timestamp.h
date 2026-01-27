//
// hinder::core
//
// MIT License
//
// Copyright (c) 2019-2026  Tony Walker
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

#ifndef HINDER_CORE_TIMESTAMP_H
#define HINDER_CORE_TIMESTAMP_H

#include <chrono>
#include <format>
#include <hinder/core/compiler.h>
#include <string>

namespace hinder {

    //
    // Get the current time (UTC or any timezone) as a formatted string.
    //
    // The default format is:
    //       utc_timestamp():  "2021-04-14T14:41:26.833393854Z"
    //     local_timestamp():  "2021-04-14T10:41:26.833393854Z America/New_York"
    // You really should log in ISO formatted time. The only change I would recommend is removing
    // the 'T' separating the date from the time. See timestamp_format below.
    //
    // To set the time zone [only impacts local_timestamp()]:
    //    timestamp_format::time_zone =  std::chrono::locate_zone("Europe/Berlin");
    // If you do not set a time zone, local_timestamp() uses std::chrono::current_zone().
    //
    // These are convenience functions that probably should only be used for error/log messages.
    //

    struct timestamp_format {
        static std::string                    utc_format;
        static std::string                    local_format;
        static const std::chrono::time_zone * time_zone;
    };

    HINDER_NODISCARD auto utc_timestamp() -> std::string {
        // current time in UTC
        auto t = std::chrono::system_clock::now();

        // break into ymd and time-of-day
        auto dp  = std::chrono::floor<std::chrono::days>(t);
        auto ymd = std::chrono::year_month_day(dp);
        auto tod = std::chrono::hh_mm_ss(t - dp);

        // convert the time to a string
        auto year = static_cast<int>(ymd.year());
        auto month = static_cast<unsigned>(ymd.month());
        auto day = static_cast<unsigned>(ymd.day());
        auto hours = tod.hours().count();
        auto minutes = tod.minutes().count();
        auto seconds = tod.seconds().count();
        auto subseconds = tod.subseconds().count();

        return std::vformat(timestamp_format::utc_format,
                            std::make_format_args(year, month, day, hours, minutes, seconds, subseconds));
    }

    HINDER_NODISCARD auto local_timestamp() -> std::string {
        // current time in desired time zone
        if (timestamp_format::time_zone == nullptr) {
            timestamp_format::time_zone = std::chrono::current_zone();
        }
        auto t = std::chrono::zoned_time(timestamp_format::time_zone, std::chrono::system_clock::now());

        // break into ymd and time-of-day
        auto dp  = std::chrono::floor<std::chrono::days>(t.get_local_time());
        auto ymd = std::chrono::year_month_day(dp);
        auto tod = std::chrono::hh_mm_ss(t.get_local_time() - dp);

        // convert the time to a string
        auto year = static_cast<int>(ymd.year());
        auto month = static_cast<unsigned>(ymd.month());
        auto day = static_cast<unsigned>(ymd.day());
        auto hours = tod.hours().count();
        auto minutes = tod.minutes().count();
        auto seconds = tod.seconds().count();
        auto subseconds = tod.subseconds().count();
        auto tz_name = t.get_time_zone()->name();

        return std::vformat(timestamp_format::local_format,
                            std::make_format_args(year, month, day, hours, minutes, seconds, subseconds, tz_name));
    }

}  // namespace hinder

#endif  // HINDER_CORE_TIMESTAMP_H
