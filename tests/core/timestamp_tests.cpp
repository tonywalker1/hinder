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

#include <catch2/catch.hpp>
#include <chrono>
#include <format>

using namespace std::chrono;

TEST_CASE("utc_timestamp with default ISO format", "[timestamp]") {
    // Test with a known time: 2021-04-14 14:41:26.833393854 UTC
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s + 833'393'854ns;

    auto result = hinder::utc_timestamp(hinder::utc_timestamp_config::iso_format, tp);

    REQUIRE(result == "2021-04-14T14:41:26.833393854Z");
}

TEST_CASE("utc_timestamp with custom format", "[timestamp]") {
    // Test with a known time: 2023-12-25 08:30:00.123456789 UTC
    auto ymd = 2023y / December / 25d;
    auto tp  = sys_days{ymd} + 8h + 30min + 0s + 123'456'789ns;

    auto config = hinder::utc_timestamp_config{"{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}"};
    auto result = hinder::utc_timestamp(config, tp);

    REQUIRE(result == "2023/12/25 08:30:00");
}

TEST_CASE("utc_timestamp with leap year date", "[timestamp]") {
    // Test with a leap year date: 2024-02-29 23:59:59.999999999 UTC
    auto ymd = 2024y / February / 29d;
    auto tp  = sys_days{ymd} + 23h + 59min + 59s + 999'999'999ns;

    auto result = hinder::utc_timestamp(hinder::utc_timestamp_config::iso_format, tp);

    REQUIRE(result == "2024-02-29T23:59:59.999999999Z");
}

TEST_CASE("utc_timestamp with epoch time", "[timestamp]") {
    // Test with Unix epoch: 1970-01-01 00:00:00.000000000 UTC
    auto tp = system_clock::time_point{};

    auto result = hinder::utc_timestamp(hinder::utc_timestamp_config::iso_format, tp);

    REQUIRE(result == "1970-01-01T00:00:00.000000000Z");
}

TEST_CASE("utc_timestamp with invalid format string", "[timestamp]") {
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s;

    // Format string expects more arguments than provided
    auto config = hinder::utc_timestamp_config{"{} {} {} {} {} {} {} {}"};

    REQUIRE_THROWS_AS(hinder::utc_timestamp(config, tp), std::format_error);
}

TEST_CASE("utc_timestamp with mismatched format specifiers", "[timestamp]") {
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s;

    // Format string expects string but gets integer
    auto config = hinder::utc_timestamp_config{"{:s}"};

    REQUIRE_THROWS_AS(hinder::utc_timestamp(config, tp), std::format_error);
}

TEST_CASE("local_timestamp with default ISO format using current timezone", "[timestamp]") {
    // Test with a known time: 2021-04-14 14:41:26.833393854
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s + 833'393'854ns;

    auto result = hinder::local_timestamp(hinder::local_timestamp_config::iso_format, tp);

    // Result should contain the timestamp and timezone name
    // We can't predict the exact timezone, but we can verify the format structure
    REQUIRE(result.find("2021-04-14T") == 0);
    REQUIRE(result.find('.') != std::string::npos);
    REQUIRE(result.rfind(' ') != std::string::npos);  // Space before timezone name
}

TEST_CASE("local_timestamp with UTC timezone", "[timestamp]") {
    // Test with a known time in UTC: 2022-06-15 10:30:45.500000000
    auto ymd = 2022y / June / 15d;
    auto tp  = sys_days{ymd} + 10h + 30min + 45s + 500'000'000ns;

    auto config = hinder::local_timestamp_config{
        "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d} {}",
        locate_zone("UTC")};
    auto result = hinder::local_timestamp(config, tp);

    // UTC timezone canonical name is "Etc/UTC"
    REQUIRE(result == "2022-06-15T10:30:45.500000000 Etc/UTC");
}

TEST_CASE("local_timestamp with America/New_York timezone", "[timestamp]") {
    // Test with a known UTC time: 2021-07-04 12:00:00 UTC
    // In America/New_York (EDT), this would be 08:00:00 (UTC-4 during summer)
    auto ymd = 2021y / July / 4d;
    auto tp  = sys_days{ymd} + 12h + 0min + 0s;

    auto config = hinder::local_timestamp_config{
        "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d} {}",
        locate_zone("America/New_York")};
    auto result = hinder::local_timestamp(config, tp);

    // EDT is UTC-4 during summer, so 12:00 UTC = 08:00 EDT
    REQUIRE(result == "2021-07-04T08:00:00.000000000 America/New_York");
}

TEST_CASE("local_timestamp with Europe/Berlin timezone", "[timestamp]") {
    // Test with a known UTC time: 2021-01-15 12:00:00 UTC
    // In Europe/Berlin (CET), this would be 13:00:00 (UTC+1 during winter)
    auto ymd = 2021y / January / 15d;
    auto tp  = sys_days{ymd} + 12h + 0min + 0s;

    auto config = hinder::local_timestamp_config{
        "{}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}.{:09d} {}",
        locate_zone("Europe/Berlin")};
    auto result = hinder::local_timestamp(config, tp);

    // CET is UTC+1 during winter, so 12:00 UTC = 13:00 CET
    REQUIRE(result == "2021-01-15T13:00:00.000000000 Europe/Berlin");
}

TEST_CASE("local_timestamp with custom format", "[timestamp]") {
    auto ymd = 2023y / November / 11d;
    auto tp  = sys_days{ymd} + 15h + 45min + 30s;

    auto config = hinder::local_timestamp_config{
        "{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}.{:09d} {}",
        locate_zone("UTC")};
    auto result = hinder::local_timestamp(config, tp);

    // UTC timezone canonical name is "Etc/UTC"
    REQUIRE(result == "2023/11/11 15:45:30.000000000 Etc/UTC");
}

TEST_CASE("local_timestamp with invalid format string", "[timestamp]") {
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s;

    // Format string expects more arguments than provided
    auto config = hinder::local_timestamp_config{
        "{} {} {} {} {} {} {} {} {}",
        locate_zone("UTC")};

    REQUIRE_THROWS_AS(hinder::local_timestamp(config, tp), std::format_error);
}

TEST_CASE("local_timestamp with mismatched format specifiers", "[timestamp]") {
    auto ymd = 2021y / April / 14d;
    auto tp  = sys_days{ymd} + 14h + 41min + 26s;

    // Format string expects string but gets integer
    auto config = hinder::local_timestamp_config{"{:s}", locate_zone("UTC")};

    REQUIRE_THROWS_AS(hinder::local_timestamp(config, tp), std::format_error);
}

TEST_CASE("utc_timestamp uses current time when called with no time argument", "[timestamp]") {
    // Call without time argument - should use current time
    auto result1 = hinder::utc_timestamp();
    auto result2 = hinder::utc_timestamp();

    // Both should be valid ISO format strings
    REQUIRE(result1.find('T') != std::string::npos);
    REQUIRE(result1.find('Z') != std::string::npos);
    REQUIRE(result2.find('T') != std::string::npos);
    REQUIRE(result2.find('Z') != std::string::npos);

    // They might be the same or different depending on timing
    // Just verify they're both reasonable length
    REQUIRE(result1.length() >= 20);
    REQUIRE(result2.length() >= 20);
}

TEST_CASE("local_timestamp uses current time when called with no time argument", "[timestamp]") {
    // Call without time argument - should use current time
    auto result1 = hinder::local_timestamp();
    auto result2 = hinder::local_timestamp();

    // Both should be valid ISO format strings with timezone
    REQUIRE(result1.find('T') != std::string::npos);
    REQUIRE(result1.rfind(' ') != std::string::npos);
    REQUIRE(result2.find('T') != std::string::npos);
    REQUIRE(result2.rfind(' ') != std::string::npos);

    // They might be the same or different depending on timing
    // Just verify they're both reasonable length
    REQUIRE(result1.length() >= 20);
    REQUIRE(result2.length() >= 20);
}
