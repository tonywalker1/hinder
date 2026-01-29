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
#include <string>

namespace hinder {

    //
    // Get the current time (UTC or any timezone) as a formatted string.
    //
    // The default format is:
    //       utc_timestamp():  "2021-04-14T14:41:26.833393854Z"
    //     local_timestamp():  "2021-04-14T10:41:26.833393854Z America/New_York"
    //
    // To customize the format or timezone, create a config object:
    //     auto berlin_config = local_timestamp_config{
    //         "{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d} {}",
    //         std::chrono::locate_zone("Europe/Berlin")
    //     };
    //     auto ts = local_timestamp(berlin_config);
    //
    // These are convenience functions that probably should only be used for error/log messages.
    //

    struct utc_timestamp_config {
        std::string format;

        explicit utc_timestamp_config(std::string fmt) : format(std::move(fmt)) {}

        static const utc_timestamp_config iso_format;
    };

    struct local_timestamp_config {
        std::string                    format;
        const std::chrono::time_zone * timezone;

        local_timestamp_config(std::string fmt, const std::chrono::time_zone * tz = nullptr)
        : format(std::move(fmt)),
          timezone(tz) {}

        static const local_timestamp_config iso_format;
    };

    [[nodiscard]] auto
        utc_timestamp(const utc_timestamp_config & config = utc_timestamp_config::iso_format)
            -> std::string;

    [[nodiscard]] auto
        local_timestamp(const local_timestamp_config & config = local_timestamp_config::iso_format)
            -> std::string;

}  // namespace hinder

#endif  // HINDER_CORE_TIMESTAMP_H
