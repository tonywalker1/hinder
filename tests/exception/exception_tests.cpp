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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <hinder/exception/exception.h>
#include <cstdint>
#include <string>

using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::EndsWith;
using namespace hinder;

// ============================================================================
// Exception creation and data storage
// ============================================================================

TEST(Exception, ExceptionCreationWithWithChaining) {
    try {
        throw generic_error()
            .message("Something went wrong")
            .with("path", "/tmp/file.txt")
            .with("errno", 2)
            .with("retries", 3U);
    } catch (generic_error const& e) {
        EXPECT_TRUE(e.contains("message"));
        EXPECT_TRUE(e.contains("path"));
        EXPECT_TRUE(e.contains("errno"));
        EXPECT_TRUE(e.contains("retries"));
        EXPECT_FALSE(e.contains("nonexistent"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, StoringBoolValues) {
    try {
        throw generic_error()
            .with("bool_val", true)
            .with("int_val", -42)
            .with("uint_val", 42U)
            .with("double_val", 3.14159)
            .with("string_val", "hello");
    } catch (generic_error const& e) {
        auto val = e.get_as<bool>("bool_val");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(*val, true);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, StoringSignedIntegerValues) {
    try {
        throw generic_error()
            .with("bool_val", true)
            .with("int_val", -42)
            .with("uint_val", 42U)
            .with("double_val", 3.14159)
            .with("string_val", "hello");
    } catch (generic_error const& e) {
        auto val = e.get_as<std::int64_t>("int_val");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(*val, -42);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, StoringUnsignedIntegerValues) {
    try {
        throw generic_error()
            .with("bool_val", true)
            .with("int_val", -42)
            .with("uint_val", 42U)
            .with("double_val", 3.14159)
            .with("string_val", "hello");
    } catch (generic_error const& e) {
        auto val = e.get_as<std::uint64_t>("uint_val");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(*val, 42U);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, StoringDoubleValues) {
    try {
        throw generic_error()
            .with("bool_val", true)
            .with("int_val", -42)
            .with("uint_val", 42U)
            .with("double_val", 3.14159)
            .with("string_val", "hello");
    } catch (generic_error const& e) {
        auto val = e.get_as<double>("double_val");
        ASSERT_TRUE(val.has_value());
        EXPECT_NEAR(*val, 3.14159, 0.00001);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, StoringStringValues) {
    try {
        throw generic_error()
            .with("bool_val", true)
            .with("int_val", -42)
            .with("uint_val", 42U)
            .with("double_val", 3.14159)
            .with("string_val", "hello");
    } catch (generic_error const& e) {
        auto val = e.get_as<std::string>("string_val");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(*val, "hello");
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, FlagstyleKeyWithNoValue) {
    try {
        throw generic_error().with("debug_mode");
    } catch (generic_error const& e) {
        EXPECT_TRUE(e.contains("debug_mode"));
        auto val = e.get("debug_mode");
        ASSERT_TRUE(val.has_value());
        EXPECT_TRUE(std::holds_alternative<std::monostate>(*val));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, SourceLocationIsCaptured) {
    try {
        throw generic_error().message("test");
    } catch (generic_error const& e) {
        auto const& loc = e.location();
        EXPECT_NE(std::string(loc.file_name()).find("exception_tests.cpp"), std::string::npos);
        EXPECT_TRUE(loc.line() > 0);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

// ============================================================================
// Data retrieval
// ============================================================================

TEST(Exception, GettingValuesThatExist) {
    try {
        throw generic_error()
            .with("count", 42)
            .with("name", "test");
    } catch (generic_error const& e) {
        auto count = e.get("count");
        ASSERT_TRUE(count.has_value());
        EXPECT_TRUE(std::holds_alternative<std::int64_t>(*count));

        auto count_int = e.get_as<int>("count");
        ASSERT_TRUE(count_int.has_value());
        EXPECT_EQ(*count_int, 42);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, GettingMissingKeys) {
    try {
        throw generic_error();
    } catch (generic_error const& e) {
        EXPECT_FALSE(e.get("missing").has_value());
        EXPECT_FALSE(e.get_as<int>("missing").has_value());
        EXPECT_FALSE(e.contains("missing"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, IteratingOverKeyvaluePairs) {
    try {
        throw generic_error()
            .with("a", 1)
            .with("b", 2)
            .with("c", 3);
    } catch (generic_error const& e) {
        EXPECT_EQ(e.size(), 3);
        int count = 0;
        for (auto const& [key, value] : e) {
            (void)key;
            (void)value;
            ++count;
        }
        EXPECT_EQ(count, 3);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

// ============================================================================
// Formatting
// ============================================================================

TEST(Exception, To_stringFormatting) {
    try {
        throw generic_error()
            .message("Failed to open file")
            .with("path", "/etc/config.json")
            .with("errno", 2);
    } catch (exception const& e) {
        auto result = to_string(e);

        EXPECT_THAT(result, HasSubstr("generic_error"));
        EXPECT_THAT(result, HasSubstr("@"));
        EXPECT_THAT(result, HasSubstr("exception_tests.cpp:"));
        EXPECT_THAT(result, HasSubstr("message: Failed to open file"));
        EXPECT_THAT(result, HasSubstr("path: /etc/config.json"));
        EXPECT_THAT(result, HasSubstr("errno: 2"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, To_stringEmptyException) {
    try {
        throw generic_error();
    } catch (exception const& e) {
        auto result = to_string(e);
        EXPECT_THAT(result, HasSubstr("generic_error"));
        EXPECT_THAT(result, HasSubstr("@"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, To_jsonFormatting) {
    try {
        throw generic_error()
            .message("Test message")
            .with("count", 42)
            .with("enabled", true);
    } catch (exception const& e) {
        auto result = to_json(e);

        EXPECT_THAT(result, StartsWith("{"));
        EXPECT_THAT(result, EndsWith("}"));
        EXPECT_THAT(result, HasSubstr("\"type\":\"generic_error\""));
        EXPECT_THAT(result, HasSubstr("\"source\":{"));
        EXPECT_THAT(result, HasSubstr("\"file\":"));
        EXPECT_THAT(result, HasSubstr("\"line\":"));
        EXPECT_THAT(result, HasSubstr("\"data\":{"));
        EXPECT_THAT(result, HasSubstr("\"message\":\"Test message\""));
        EXPECT_THAT(result, HasSubstr("\"count\":42"));
        EXPECT_THAT(result, HasSubstr("\"enabled\":true"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, JsonEscapingSpecialCharacters) {
    try {
        throw generic_error()
            .with("quotes", "say \"hello\"")
            .with("backslash", "path\\to\\file")
            .with("newline", "line1\nline2")
            .with("tab", "col1\tcol2");
    } catch (exception const& e) {
        auto result = to_json(e);

        EXPECT_THAT(result, HasSubstr("say \\\"hello\\\""));
        EXPECT_THAT(result, HasSubstr("path\\\\to\\\\file"));
        EXPECT_THAT(result, HasSubstr("line1\\nline2"));
        EXPECT_THAT(result, HasSubstr("col1\\tcol2"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, JsonEscapingControlCharacters) {
    try {
        throw generic_error().with("bell", std::string("ring\x07"));
    } catch (exception const& e) {
        auto result = to_json(e);
        EXPECT_THAT(result, HasSubstr("ring\\u0007"));
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

// ============================================================================
// Macros
// ============================================================================

TEST(Exception, Hinder_throwCapturesSourceLocation) {
    try {
        HINDER_THROW(generic_error).message("test");
    } catch (exception const& e) {
        EXPECT_NE(std::string(e.location().file_name()).find("exception_tests.cpp"), std::string::npos);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, Hinder_throwTypeHierarchy) {
    EXPECT_THROW(HINDER_THROW(generic_error), generic_error);
    EXPECT_THROW(HINDER_THROW(generic_error), exception);
    EXPECT_THROW(HINDER_THROW(generic_error), std::runtime_error);
    EXPECT_THROW(HINDER_THROW(generic_error), std::exception);
}

TEST(Exception, Hinder_expectsThrowsOnFalse) {
    try {
        int x = 0;
        HINDER_EXPECTS(x > 0, generic_error);
        FAIL() << "Expected exception";
    } catch (exception const& e) {
        EXPECT_EQ(e.get_as<std::string>("condition").value_or(""), "x > 0");
        EXPECT_EQ(e.get_as<std::string>("check_type").value_or(""), "precondition");
    }
}

TEST(Exception, Hinder_expectsPassesOnTrue) {
    EXPECT_NO_THROW(HINDER_EXPECTS(true, generic_error));
}

TEST(Exception, Hinder_ensuresThrowsOnFalse) {
    try {
        HINDER_ENSURES(1 == 2, generic_error);
        FAIL() << "Expected exception";
    } catch (exception const& e) {
        EXPECT_EQ(e.get_as<std::string>("condition").value_or(""), "1 == 2");
        EXPECT_EQ(e.get_as<std::string>("check_type").value_or(""), "postcondition");
    }
}

TEST(Exception, Hinder_ensuresPassesOnTrue) {
    EXPECT_NO_THROW(HINDER_ENSURES(true, generic_error));
}

TEST(Exception, Hinder_invariantThrowsOnFalse) {
    try {
        HINDER_INVARIANT(false, generic_error);
        FAIL() << "Expected exception";
    } catch (exception const& e) {
        EXPECT_EQ(e.get_as<std::string>("condition").value_or(""), "false");
        EXPECT_EQ(e.get_as<std::string>("check_type").value_or(""), "invariant");
    }
}

TEST(Exception, Hinder_invariantPassesOnTrue) {
    EXPECT_NO_THROW(HINDER_INVARIANT(true, generic_error));
}

// ============================================================================
// Inheritance
// ============================================================================

HINDER_DEFINE_EXCEPTION(custom_error, generic_error);
HINDER_DEFINE_EXCEPTION(specific_error, custom_error);

TEST(Exception, CustomExceptionTypesAreCatchableByType) {
    EXPECT_THROW(throw custom_error(), custom_error);
    EXPECT_THROW(throw specific_error(), specific_error);
}

TEST(Exception, CustomExceptionTypesAreCatchableByBaseTypes) {
    EXPECT_THROW(throw custom_error(), generic_error);
    EXPECT_THROW(throw custom_error(), exception);
    EXPECT_THROW(throw specific_error(), custom_error);
    EXPECT_THROW(throw specific_error(), generic_error);
}

TEST(Exception, Type_nameReturnsCorrectName) {
    try {
        throw custom_error();
    } catch (exception const& e) {
        EXPECT_EQ(e.type_name(), "custom_error");
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, WhatReturnsTypeName) {
    try {
        throw custom_error();
    } catch (std::exception const& e) {
        EXPECT_EQ(std::string(e.what()), "custom_error");
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

// ============================================================================
// Edge cases
// ============================================================================

TEST(Exception, EmptyException) {
    try {
        throw generic_error();
    } catch (exception const& e) {
        EXPECT_EQ(e.size(), 0);
        EXPECT_EQ(e.begin(), e.end());
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, LargeStringValues) {
    std::string large_string(10000, 'x');
    try {
        throw generic_error().with("large", large_string);
    } catch (exception const& e) {
        auto val = e.get_as<std::string>("large");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(val->size(), 10000);
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, UnicodeStrings) {
    try {
        throw generic_error()
            .with("emoji", "Hello 🌍")
            .with("chinese", "你好世界")
            .with("mixed", "Héllo Wörld");
    } catch (exception const& e) {
        EXPECT_EQ(e.get_as<std::string>("emoji").value_or(""), "Hello 🌍");
        EXPECT_EQ(e.get_as<std::string>("chinese").value_or(""), "你好世界");
        EXPECT_EQ(e.get_as<std::string>("mixed").value_or(""), "Héllo Wörld");
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

TEST(Exception, MessageWithFormatArguments) {
    try {
        throw generic_error().message("Value {} exceeds limit {}", 42, 100);
    } catch (exception const& e) {
        EXPECT_EQ(e.get_as<std::string>("message").value_or(""), "Value 42 exceeds limit 100");
        return;
    }
    FAIL() << "Expected exception to be thrown";
}

// ============================================================================
// value_to_string
// ============================================================================

TEST(Exception, Value_to_stringMonostate) {
    EXPECT_EQ(value_to_string(std::monostate{}), "");
}

TEST(Exception, Value_to_stringBool) {
    EXPECT_EQ(value_to_string(true), "true");
    EXPECT_EQ(value_to_string(false), "false");
}

TEST(Exception, Value_to_stringIntegers) {
    EXPECT_EQ(value_to_string(std::int64_t{-42}), "-42");
    EXPECT_EQ(value_to_string(std::uint64_t{42}), "42");
}

TEST(Exception, Value_to_stringDouble) {
    EXPECT_EQ(value_to_string(3.14), "3.14");
}

TEST(Exception, Value_to_stringString) {
    EXPECT_EQ(value_to_string(std::string{"hello"}), "hello");
}

// ============================================================================
// HINDER_ASSERT (debug-only assertions)
// ============================================================================

#ifndef NDEBUG

TEST(Assert, HinderassertThrowsOnFalseCondition) {
    EXPECT_THROW(HINDER_ASSERT(1 == 0, "condition is false"), assertion_error);
}

TEST(Assert, HinderassertPassesOnTrueCondition) {
    EXPECT_NO_THROW(HINDER_ASSERT(1 == 1, "condition is true"));
}

TEST(Assert, HinderassertCapturesConditionAndMessage) {
    try {
        int x = 42;
        HINDER_ASSERT(x == 0, "expected {}, got {}", 0, x);
        FAIL() << "Expected assertion_error";
    } catch (assertion_error const& e) {
        EXPECT_EQ(e.get_as<std::string>("condition").value_or(""), "x == 0");
        EXPECT_EQ(e.get_as<std::string>("check_type").value_or(""), "assertion");
        auto msg = e.get_as<std::string>("message").value_or("");
        EXPECT_THAT(msg, HasSubstr("expected 0"));
        EXPECT_THAT(msg, HasSubstr("got 42"));
    }
}

TEST(Assert, HinderassertCapturesSourceLocation) {
    try {
        HINDER_ASSERT(false, "test assertion");
        FAIL() << "Expected assertion_error";
    } catch (assertion_error const& e) {
        auto const& loc = e.location();
        EXPECT_NE(std::string(loc.file_name()).find("exception_tests.cpp"), std::string::npos);
        EXPECT_TRUE(loc.line() > 0);
    }
}

#endif  // !NDEBUG
