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

#include <hinder/timestamp.h>

#include <chrono>
#include <format>

namespace hinder {

    // Static member definitions
    const utc_timestamp_config utc_timestamp_config::iso_format {
        "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d}Z"};

    const local_timestamp_config local_timestamp_config::iso_format {
        "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d} {}",
        nullptr  // Resolved to std::chrono::current_zone() at call time
    };

    // Function implementations
    auto utc_timestamp(const utc_timestamp_config &                config,
                       const std::chrono::system_clock::time_point now) -> std::string {

        // break into ymd and time-of-day
        auto dp  = std::chrono::floor<std::chrono::days>(now);
        auto ymd = std::chrono::year_month_day(dp);
        auto tod = std::chrono::hh_mm_ss(now - dp);

        // convert the time to a string
        auto year       = static_cast<int>(ymd.year());
        auto month      = static_cast<unsigned>(ymd.month());
        auto day        = static_cast<unsigned>(ymd.day());
        auto hours      = tod.hours().count();
        auto minutes    = tod.minutes().count();
        auto seconds    = tod.seconds().count();
        auto subseconds = tod.subseconds().count();

        return std::vformat(
            config.format,
            std::make_format_args(year, month, day, hours, minutes, seconds, subseconds));
    }

    auto local_timestamp(const local_timestamp_config &              config,
                         const std::chrono::system_clock::time_point now) -> std::string {
        // current time in desired time zone
        // Resolve nullptr to current_zone() at call time (handles TZ environment changes)
        auto tz = config.timezone != nullptr ? config.timezone : std::chrono::current_zone();
        auto t  = std::chrono::zoned_time(tz, now);

        // break into ymd and time-of-day
        auto dp  = std::chrono::floor<std::chrono::days>(t.get_local_time());
        auto ymd = std::chrono::year_month_day(dp);
        auto tod = std::chrono::hh_mm_ss(t.get_local_time() - dp);

        // convert the time to a string
        auto year       = static_cast<int>(ymd.year());
        auto month      = static_cast<unsigned>(ymd.month());
        auto day        = static_cast<unsigned>(ymd.day());
        auto hours      = tod.hours().count();
        auto minutes    = tod.minutes().count();
        auto seconds    = tod.seconds().count();
        auto subseconds = tod.subseconds().count();
        auto tz_name    = t.get_time_zone()->name();

        return std::vformat(
            config.format,
            std::make_format_args(year, month, day, hours, minutes, seconds, subseconds, tz_name));
    }

}  // namespace hinder
