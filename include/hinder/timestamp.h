#pragma once

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

#include <chrono>
#include <string>

namespace hinder {

    //
    // Get the current time (UTC or any timezone) as a formatted string.
    //
    // The default format is:
    //       utc_timestamp()():  "2021-04-14T14:41:26.833393854Z"
    //     local_timestamp()():  "2021-04-14T10:41:26.833393854 America/New_York"
    //
    // utc_ts and local_ts are callable objects (functors) that capture their format
    // and timezone at construction and produce strings on demand:
    //     utc_timestamp()();                    // default format, current time
    //     utc_timestamp()(test_time);           // default format, explicit time (for testing)
    //
    //     auto berlin = local_ts{fmt, std::chrono::locate_zone("Europe/Berlin")};
    //     berlin();                             // custom zone, current time
    //     berlin(test_time);                    // custom zone, explicit time
    //
    // The format string is validated at construction by doing a trial call with the
    // epoch time point. An invalid format string throws hinder::generic_error rather
    // than deferring the error to the first operator() call.
    //
    // These are convenience functions that probably should only be used for error/log messages.
    //

    class utc_ts {
    public:
        explicit utc_ts(std::string fmt = "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d}Z");

        [[nodiscard]] auto operator()(
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now()) const
            -> std::string;

    private:
        std::string m_format;
    };

    class local_ts {
    public:
        explicit local_ts(
            std::string                    fmt  = "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d} {}",
            const std::chrono::time_zone * zone = nullptr);

        [[nodiscard]] auto operator()(
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now()) const
            -> std::string;

    private:
        std::string                    m_format;
        const std::chrono::time_zone * m_timezone;
    };

    [[nodiscard]] auto utc_timestamp() -> utc_ts const &;
    [[nodiscard]] auto local_timestamp() -> local_ts const &;

}  // namespace hinder
