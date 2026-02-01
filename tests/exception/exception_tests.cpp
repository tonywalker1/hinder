//
// hinder::exception
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

#include <catch2/catch.hpp>
#include <hinder/exception/exception.h>
#include <cstdint>
#include <string>

using namespace Catch::Matchers;
using namespace hinder;

// ============================================================================
// Exception creation and data storage
// ============================================================================

TEST_CASE("Exception creation with .with() chaining", "[exception]") {
    try {
        throw generic_error()
            .message("Something went wrong")
            .with("path", "/tmp/file.txt")
            .with("errno", 2)
            .with("retries", 3U);
    } catch (generic_error const& e) {
        CHECK(e.contains("message"));
        CHECK(e.contains("path"));
        CHECK(e.contains("errno"));
        CHECK(e.contains("retries"));
        CHECK(!e.contains("nonexistent"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Storing different value types", "[exception]") {
    try {
        throw generic_error()
            .with("bool_val", true)
            .with("int_val", -42)
            .with("uint_val", 42U)
            .with("double_val", 3.14159)
            .with("string_val", "hello");
    } catch (generic_error const& e) {
        SECTION("bool values") {
            auto val = e.get_as<bool>("bool_val");
            REQUIRE(val.has_value());
            CHECK(*val == true);
        }
        SECTION("signed integer values") {
            auto val = e.get_as<std::int64_t>("int_val");
            REQUIRE(val.has_value());
            CHECK(*val == -42);
        }
        SECTION("unsigned integer values") {
            auto val = e.get_as<std::uint64_t>("uint_val");
            REQUIRE(val.has_value());
            CHECK(*val == 42U);
        }
        SECTION("double values") {
            auto val = e.get_as<double>("double_val");
            REQUIRE(val.has_value());
            CHECK(*val == Catch::Detail::Approx(3.14159));
        }
        SECTION("string values") {
            auto val = e.get_as<std::string>("string_val");
            REQUIRE(val.has_value());
            CHECK(*val == "hello");
        }
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Flag-style key with no value", "[exception]") {
    try {
        throw generic_error().with("debug_mode");
    } catch (generic_error const& e) {
        CHECK(e.contains("debug_mode"));
        auto val = e.get("debug_mode");
        REQUIRE(val.has_value());
        CHECK(std::holds_alternative<std::monostate>(*val));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Source location is captured", "[exception]") {
    try {
        throw generic_error().message("test");
    } catch (generic_error const& e) {
        auto const& loc = e.location();
        CHECK(std::string(loc.file_name()).find("exception_tests.cpp") != std::string::npos);
        CHECK(loc.line() > 0);
        return;
    }
    FAIL("Expected exception to be thrown");
}

// ============================================================================
// Data retrieval
// ============================================================================

TEST_CASE("Getting values that exist", "[exception]") {
    try {
        throw generic_error()
            .with("count", 42)
            .with("name", "test");
    } catch (generic_error const& e) {
        auto count = e.get("count");
        REQUIRE(count.has_value());
        CHECK(std::holds_alternative<std::int64_t>(*count));

        auto count_int = e.get_as<int>("count");
        REQUIRE(count_int.has_value());
        CHECK(*count_int == 42);
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Getting missing keys", "[exception]") {
    try {
        throw generic_error();
    } catch (generic_error const& e) {
        CHECK(!e.get("missing").has_value());
        CHECK(!e.get_as<int>("missing").has_value());
        CHECK(!e.contains("missing"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Iterating over key-value pairs", "[exception]") {
    try {
        throw generic_error()
            .with("a", 1)
            .with("b", 2)
            .with("c", 3);
    } catch (generic_error const& e) {
        CHECK(e.size() == 3);
        int count = 0;
        for (auto const& [key, value] : e) {
            (void)key;
            (void)value;
            ++count;
        }
        CHECK(count == 3);
        return;
    }
    FAIL("Expected exception to be thrown");
}

// ============================================================================
// Formatting
// ============================================================================

TEST_CASE("to_string() formatting", "[exception]") {
    try {
        throw generic_error()
            .message("Failed to open file")
            .with("path", "/etc/config.json")
            .with("errno", 2);
    } catch (exception const& e) {
        auto result = to_string(e);

        CHECK_THAT(result, Contains("generic_error"));
        CHECK_THAT(result, Contains("@"));
        CHECK_THAT(result, Contains("exception_tests.cpp:"));
        CHECK_THAT(result, Contains("message: Failed to open file"));
        CHECK_THAT(result, Contains("path: /etc/config.json"));
        CHECK_THAT(result, Contains("errno: 2"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("to_string() empty exception", "[exception]") {
    try {
        throw generic_error();
    } catch (exception const& e) {
        auto result = to_string(e);
        CHECK_THAT(result, Contains("generic_error"));
        CHECK_THAT(result, Contains("@"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("to_json() formatting", "[exception]") {
    try {
        throw generic_error()
            .message("Test message")
            .with("count", 42)
            .with("enabled", true);
    } catch (exception const& e) {
        auto result = to_json(e);

        CHECK_THAT(result, StartsWith("{"));
        CHECK_THAT(result, EndsWith("}"));
        CHECK_THAT(result, Contains("\"type\":\"generic_error\""));
        CHECK_THAT(result, Contains("\"source\":{"));
        CHECK_THAT(result, Contains("\"file\":"));
        CHECK_THAT(result, Contains("\"line\":"));
        CHECK_THAT(result, Contains("\"data\":{"));
        CHECK_THAT(result, Contains("\"message\":\"Test message\""));
        CHECK_THAT(result, Contains("\"count\":42"));
        CHECK_THAT(result, Contains("\"enabled\":true"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("JSON escaping special characters", "[exception]") {
    try {
        throw generic_error()
            .with("quotes", "say \"hello\"")
            .with("backslash", "path\\to\\file")
            .with("newline", "line1\nline2")
            .with("tab", "col1\tcol2");
    } catch (exception const& e) {
        auto result = to_json(e);

        CHECK_THAT(result, Contains("say \\\"hello\\\""));
        CHECK_THAT(result, Contains("path\\\\to\\\\file"));
        CHECK_THAT(result, Contains("line1\\nline2"));
        CHECK_THAT(result, Contains("col1\\tcol2"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("JSON escaping control characters", "[exception]") {
    try {
        throw generic_error().with("bell", std::string("ring\x07"));
    } catch (exception const& e) {
        auto result = to_json(e);
        CHECK_THAT(result, Contains("ring\\u0007"));
        return;
    }
    FAIL("Expected exception to be thrown");
}

// ============================================================================
// Macros
// ============================================================================

TEST_CASE("HINDER_THROW captures source location", "[exception]") {
    try {
        HINDER_THROW(generic_error).message("test");
    } catch (exception const& e) {
        CHECK(std::string(e.location().file_name()).find("exception_tests.cpp")
              != std::string::npos);
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("HINDER_THROW type hierarchy", "[exception]") {
    CHECK_THROWS_AS(HINDER_THROW(generic_error), generic_error);
    CHECK_THROWS_AS(HINDER_THROW(generic_error), exception);
    CHECK_THROWS_AS(HINDER_THROW(generic_error), std::runtime_error);
    CHECK_THROWS_AS(HINDER_THROW(generic_error), std::exception);
}

TEST_CASE("HINDER_EXPECTS throws on false", "[exception]") {
    try {
        int x = 0;
        HINDER_EXPECTS(x > 0, generic_error);
        FAIL("Expected exception");
    } catch (exception const& e) {
        CHECK(e.get_as<std::string>("condition").value_or("") == "x > 0");
        CHECK(e.get_as<std::string>("check_type").value_or("") == "precondition");
    }
}

TEST_CASE("HINDER_EXPECTS passes on true", "[exception]") {
    CHECK_NOTHROW(HINDER_EXPECTS(true, generic_error));
}

TEST_CASE("HINDER_ENSURES throws on false", "[exception]") {
    try {
        HINDER_ENSURES(1 == 2, generic_error);
        FAIL("Expected exception");
    } catch (exception const& e) {
        CHECK(e.get_as<std::string>("condition").value_or("") == "1 == 2");
        CHECK(e.get_as<std::string>("check_type").value_or("") == "postcondition");
    }
}

TEST_CASE("HINDER_ENSURES passes on true", "[exception]") {
    CHECK_NOTHROW(HINDER_ENSURES(true, generic_error));
}

TEST_CASE("HINDER_INVARIANT throws on false", "[exception]") {
    try {
        HINDER_INVARIANT(false, generic_error);
        FAIL("Expected exception");
    } catch (exception const& e) {
        CHECK(e.get_as<std::string>("condition").value_or("") == "false");
        CHECK(e.get_as<std::string>("check_type").value_or("") == "invariant");
    }
}

TEST_CASE("HINDER_INVARIANT passes on true", "[exception]") {
    CHECK_NOTHROW(HINDER_INVARIANT(true, generic_error));
}

// ============================================================================
// Inheritance
// ============================================================================

HINDER_DEFINE_EXCEPTION(custom_error, generic_error);
HINDER_DEFINE_EXCEPTION(specific_error, custom_error);

TEST_CASE("Custom exception types are catchable by type", "[exception]") {
    CHECK_THROWS_AS(throw custom_error(), custom_error);
    CHECK_THROWS_AS(throw specific_error(), specific_error);
}

TEST_CASE("Custom exception types are catchable by base types", "[exception]") {
    CHECK_THROWS_AS(throw custom_error(), generic_error);
    CHECK_THROWS_AS(throw custom_error(), exception);
    CHECK_THROWS_AS(throw specific_error(), custom_error);
    CHECK_THROWS_AS(throw specific_error(), generic_error);
}

TEST_CASE("type_name() returns correct name", "[exception]") {
    try {
        throw custom_error();
    } catch (exception const& e) {
        CHECK(e.type_name() == "custom_error");
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("what() returns type name", "[exception]") {
    try {
        throw custom_error();
    } catch (std::exception const& e) {
        CHECK(std::string(e.what()) == "custom_error");
        return;
    }
    FAIL("Expected exception to be thrown");
}

// ============================================================================
// Edge cases
// ============================================================================

TEST_CASE("Empty exception", "[exception]") {
    try {
        throw generic_error();
    } catch (exception const& e) {
        CHECK(e.size() == 0);
        CHECK(e.begin() == e.end());
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Large string values", "[exception]") {
    std::string large_string(10000, 'x');
    try {
        throw generic_error().with("large", large_string);
    } catch (exception const& e) {
        auto val = e.get_as<std::string>("large");
        REQUIRE(val.has_value());
        CHECK(val->size() == 10000);
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("Unicode strings", "[exception]") {
    try {
        throw generic_error()
            .with("emoji", "Hello 🌍")
            .with("chinese", "你好世界")
            .with("mixed", "Héllo Wörld");
    } catch (exception const& e) {
        CHECK(e.get_as<std::string>("emoji").value_or("") == "Hello 🌍");
        CHECK(e.get_as<std::string>("chinese").value_or("") == "你好世界");
        CHECK(e.get_as<std::string>("mixed").value_or("") == "Héllo Wörld");
        return;
    }
    FAIL("Expected exception to be thrown");
}

TEST_CASE("message() with format arguments", "[exception]") {
    try {
        throw generic_error().message("Value {} exceeds limit {}", 42, 100);
    } catch (exception const& e) {
        CHECK(e.get_as<std::string>("message").value_or("") == "Value 42 exceeds limit 100");
        return;
    }
    FAIL("Expected exception to be thrown");
}

// ============================================================================
// value_to_string
// ============================================================================

TEST_CASE("value_to_string monostate", "[exception]") {
    CHECK(value_to_string(std::monostate{}) == "");
}

TEST_CASE("value_to_string bool", "[exception]") {
    CHECK(value_to_string(true) == "true");
    CHECK(value_to_string(false) == "false");
}

TEST_CASE("value_to_string integers", "[exception]") {
    CHECK(value_to_string(std::int64_t{-42}) == "-42");
    CHECK(value_to_string(std::uint64_t{42}) == "42");
}

TEST_CASE("value_to_string double", "[exception]") {
    CHECK(value_to_string(3.14) == "3.14");
}

TEST_CASE("value_to_string string", "[exception]") {
    CHECK(value_to_string(std::string{"hello"}) == "hello");
}

// ============================================================================
// HINDER_ASSERT (debug-only assertions)
// ============================================================================

#ifndef NDEBUG

TEST_CASE("HINDER_ASSERT throws on false condition", "[exception][assert]") {
    CHECK_THROWS_AS(HINDER_ASSERT(1 == 0, "condition is false"), assertion_error);
}

TEST_CASE("HINDER_ASSERT passes on true condition", "[exception][assert]") {
    CHECK_NOTHROW(HINDER_ASSERT(1 == 1, "condition is true"));
}

TEST_CASE("HINDER_ASSERT captures condition and message", "[exception][assert]") {
    try {
        int x = 42;
        HINDER_ASSERT(x == 0, "expected {}, got {}", 0, x);
        FAIL("Expected assertion_error");
    } catch (assertion_error const& e) {
        CHECK(e.get_as<std::string>("condition").value_or("") == "x == 0");
        CHECK(e.get_as<std::string>("check_type").value_or("") == "assertion");
        auto msg = e.get_as<std::string>("message").value_or("");
        CHECK_THAT(msg, Contains("expected 0"));
        CHECK_THAT(msg, Contains("got 42"));
    }
}

TEST_CASE("HINDER_ASSERT captures source location", "[exception][assert]") {
    try {
        HINDER_ASSERT(false, "test assertion");
        FAIL("Expected assertion_error");
    } catch (assertion_error const& e) {
        auto const& loc = e.location();
        CHECK(std::string(loc.file_name()).find("exception_tests.cpp") != std::string::npos);
        CHECK(loc.line() > 0);
    }
}

#endif  // !NDEBUG
