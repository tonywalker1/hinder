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
#include <hinder/exception/exception.h>
#include <string>
#include <utility>

namespace hinder {

    // Constructors: capture format string (and optional timezone) at construction.
    // Validate the format string eagerly by doing a trial call with the epoch time
    // point (discarding the result). This reports misconfiguration at the point where
    // the bad format string is used to construct the object, not at the first call to
    // operator(). Default instances use function-local statics; exceptions propagate
    // to the caller rather than calling std::terminate().

    utc_ts::utc_ts(std::string fmt) : m_format(std::move(fmt)) {
        try {
            static_cast<void>((*this)(std::chrono::system_clock::time_point {}));
        } catch (const std::format_error & e) {
            HINDER_THROW(generic_error)
                .message("invalid utc_ts format string: {}", e.what())
                .with("format", m_format);
        }
    }

    local_ts::local_ts(std::string fmt, const std::chrono::time_zone * zone)
    : m_format(std::move(fmt)),
      m_timezone(zone) {
        try {
            static_cast<void>((*this)(std::chrono::system_clock::time_point {}));
        } catch (const std::format_error & e) {
            HINDER_THROW(generic_error)
                .message("invalid local_ts format string: {}", e.what())
                .with("format", m_format);
        }
    }

    auto utc_ts::operator()(const std::chrono::system_clock::time_point now) const -> std::string {
        // break into ymd and time-of-day
        // NOLINTNEXTLINE(misc-include-cleaner): std::chrono::days is in <chrono>
        auto day_pt = std::chrono::floor<std::chrono::days>(now);
        auto ymd    = std::chrono::year_month_day(day_pt);
        auto tod    = std::chrono::hh_mm_ss(now - day_pt);

        // convert the time to a string
        auto year       = static_cast<int>(ymd.year());
        auto month      = static_cast<unsigned>(ymd.month());
        auto day        = static_cast<unsigned>(ymd.day());
        auto hours      = tod.hours().count();
        auto minutes    = tod.minutes().count();
        auto seconds    = tod.seconds().count();
        auto subseconds = tod.subseconds().count();

        return std::vformat(
            m_format,
            std::make_format_args(year, month, day, hours, minutes, seconds, subseconds));
    }

    auto local_ts::operator()(const std::chrono::system_clock::time_point now) const
        -> std::string {
        // current time in desired time zone
        // Resolve nullptr to current_zone() at call time (handles TZ environment changes)
        const auto * zone  = m_timezone != nullptr ? m_timezone : std::chrono::current_zone();
        auto         local = std::chrono::zoned_time(zone, now);

        // break into ymd and time-of-day
        // NOLINTNEXTLINE(misc-include-cleaner): std::chrono::days is in <chrono>
        auto day_pt = std::chrono::floor<std::chrono::days>(local.get_local_time());
        auto ymd    = std::chrono::year_month_day(day_pt);
        auto tod    = std::chrono::hh_mm_ss(local.get_local_time() - day_pt);

        // convert the time to a string
        auto year       = static_cast<int>(ymd.year());
        auto month      = static_cast<unsigned>(ymd.month());
        auto day        = static_cast<unsigned>(ymd.day());
        auto hours      = tod.hours().count();
        auto minutes    = tod.minutes().count();
        auto seconds    = tod.seconds().count();
        auto subseconds = tod.subseconds().count();
        auto tz_name    = local.get_time_zone()->name();

        return std::vformat(
            m_format,
            std::make_format_args(year, month, day, hours, minutes, seconds, subseconds, tz_name));
    }

    auto utc_timestamp() -> utc_ts const & {
        static const utc_ts instance {};
        return instance;
    }

    auto local_timestamp() -> local_ts const & {
        static const local_ts instance {};
        return instance;
    }

}  // namespace hinder
