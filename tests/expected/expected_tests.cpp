//
// Tests for hinder::error and hinder::fail()
//
// MIT License
//
// Copyright (c) 2019-2026  Tony Walker
//

#include <gtest/gtest.h>
#include <hinder/expected/error.h>

#include <cstdint>
#include <expected>
#include <format>
#include <string>
#include <utility>
#include <variant>

namespace {

    // ========================================================================
    // error construction
    // ========================================================================

    TEST(ErrorConstruction, DefaultTypeNameIsError) {
        auto err = hinder::error();
        EXPECT_EQ(err.type_name(), "error");
    }

    TEST(ErrorConstruction, CustomTypeName) {
        auto err = hinder::error("command_error");
        EXPECT_EQ(err.type_name(), "command_error");
    }

    TEST(ErrorConstruction, CapturesSourceLocation) {
        auto err = hinder::error("test_error");
        EXPECT_NE(err.location().file_name(), nullptr);
        EXPECT_GT(err.location().line(), 0U);
    }

    TEST(ErrorConstruction, EmptyByDefault) {
        auto err = hinder::error();
        EXPECT_EQ(err.size(), 0U);
    }

    // ========================================================================
    // fluent API: message() and with()
    // ========================================================================

    TEST(FluentApi, MessageSetsMessageKey) {
        auto err = hinder::error().message("something failed");
        ASSERT_TRUE(err.contains("message"));
        auto msg = err.get_as<std::string>("message");
        ASSERT_TRUE(msg.has_value());
        EXPECT_EQ(*msg, "something failed");
    }

    TEST(FluentApi, MessageWithFormatArgs) {
        auto err = hinder::error().message("exit code: {}", 42);
        auto msg = err.get_as<std::string>("message");
        ASSERT_TRUE(msg.has_value());
        EXPECT_EQ(*msg, "exit code: 42");
    }

    TEST(FluentApi, WithStringValue) {
        auto err = hinder::error().with("path", "/etc/config");
        auto val = err.get_as<std::string>("path");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(*val, "/etc/config");
    }

    TEST(FluentApi, WithIntValue) {
        auto err = hinder::error().with("code", 42);
        auto val = err.get_as<std::int64_t>("code");
        ASSERT_TRUE(val.has_value());
        EXPECT_EQ(*val, 42);
    }

    TEST(FluentApi, WithBoolValue) {
        auto err = hinder::error().with("retried", true);
        auto val = err.get_as<bool>("retried");
        ASSERT_TRUE(val.has_value());
        EXPECT_TRUE(*val);
    }

    TEST(FluentApi, WithDoubleValue) {
        auto err = hinder::error().with("ratio", 3.14);
        auto val = err.get_as<double>("ratio");
        ASSERT_TRUE(val.has_value());
        EXPECT_DOUBLE_EQ(*val, 3.14);
    }

    TEST(FluentApi, WithFlagKey) {
        auto err = hinder::error().with("timed_out");
        EXPECT_TRUE(err.contains("timed_out"));
        auto val = err.get("timed_out");
        ASSERT_TRUE(val.has_value());
        EXPECT_TRUE(std::holds_alternative<std::monostate>(*val));
    }

    TEST(FluentApi, ChainMultipleWithCalls) {
        auto err =
            hinder::error().message("failed").with("code", 1).with("path", "/tmp").with("retried");
        EXPECT_EQ(err.size(), 4U);
        EXPECT_TRUE(err.contains("message"));
        EXPECT_TRUE(err.contains("code"));
        EXPECT_TRUE(err.contains("path"));
        EXPECT_TRUE(err.contains("retried"));
    }

    // ========================================================================
    // accessors: get, get_as, contains
    // ========================================================================

    TEST(Accessors, GetMissingKeyReturnsNullopt) {
        auto err = hinder::error();
        EXPECT_FALSE(err.get("missing").has_value());
    }

    TEST(Accessors, ContainsMissingKeyReturnsFalse) {
        auto err = hinder::error();
        EXPECT_FALSE(err.contains("missing"));
    }

    TEST(Accessors, GetAsNumericConversion) {
        auto err    = hinder::error().with("count", std::int64_t {100});
        auto as_int = err.get_as<int>("count");
        ASSERT_TRUE(as_int.has_value());
        EXPECT_EQ(*as_int, 100);
    }

    TEST(Accessors, GetAsStringConversion) {
        auto err    = hinder::error().with("code", std::int64_t {42});
        auto as_str = err.get_as<std::string>("code");
        ASSERT_TRUE(as_str.has_value());
        EXPECT_EQ(*as_str, "42");
    }

    TEST(Accessors, GetAsFlagReturnsNullopt) {
        auto err = hinder::error().with("flag");
        auto val = err.get_as<std::string>("flag");
        EXPECT_FALSE(val.has_value());
    }

    // ========================================================================
    // iteration
    // ========================================================================

    TEST(Iteration, IteratesAllKeys) {
        auto err = hinder::error().with("alpha", 1).with("beta", 2).with("gamma", 3);

        int count = 0;
        for (auto const & [key, value] : err) {
            (void)key;
            (void)value;
            ++count;
        }
        EXPECT_EQ(count, 3);
    }

    // ========================================================================
    // fail() factory
    // ========================================================================

    TEST(FailFactory, DefaultTypeNameIsError) {
        auto builder    = hinder::fail();
        auto unexpected = std::unexpected<hinder::error>(std::move(builder));
        EXPECT_EQ(unexpected.error().type_name(), "error");
    }

    TEST(FailFactory, CustomTypeName) {
        auto builder    = hinder::fail("command_error");
        auto unexpected = std::unexpected<hinder::error>(std::move(builder));
        EXPECT_EQ(unexpected.error().type_name(), "command_error");
    }

    TEST(FailFactory, CapturesSourceLocation) {
        auto builder    = hinder::fail("test");
        auto unexpected = std::unexpected<hinder::error>(std::move(builder));
        EXPECT_GT(unexpected.error().location().line(), 0U);
    }

    TEST(FailFactory, FluentChaining) {
        auto builder = hinder::fail("cmd_error").message("failed: {}", "ls").with("exit_code", 1);
        auto unexpected  = std::unexpected<hinder::error>(std::move(builder));
        auto const & err = unexpected.error();
        EXPECT_EQ(err.type_name(), "cmd_error");
        EXPECT_TRUE(err.contains("message"));
        EXPECT_TRUE(err.contains("exit_code"));
    }

    // ========================================================================
    // implicit conversion to std::unexpected<error>
    // ========================================================================

    TEST(ImplicitConversion, ConvertsToUnexpected) {
        std::unexpected<hinder::error> unexpected_err = hinder::fail("my_error").message("oops");
        EXPECT_EQ(unexpected_err.error().type_name(), "my_error");
    }

    // ========================================================================
    // use in std::expected return types
    // ========================================================================

    auto divide(int numerator, int denominator) -> std::expected<int, hinder::error> {
        if (denominator == 0) {
            return hinder::fail("math_error")
                .message("division by zero")
                .with("numerator", numerator);
        }
        return numerator / denominator;
    }

    auto greet(std::string_view name) -> std::expected<std::string, hinder::error> {
        if (name.empty()) {
            return hinder::fail("input_error").message("name must not be empty");
        }
        return std::format("hello, {}!", name);
    }

    TEST(StdExpected, ReturnsValueOnSuccess) {
        auto result = divide(10, 2);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(*result, 5);
    }

    TEST(StdExpected, ReturnsErrorOnFailure) {
        auto result = divide(10, 0);
        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().type_name(), "math_error");
        EXPECT_TRUE(result.error().contains("message"));
        EXPECT_TRUE(result.error().contains("numerator"));
    }

    TEST(StdExpected, StringReturnOnSuccess) {
        auto result = greet("world");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(*result, "hello, world!");
    }

    TEST(StdExpected, StringReturnOnFailure) {
        auto result = greet("");
        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().type_name(), "input_error");
    }

    // ========================================================================
    // to_string formatting
    // ========================================================================

    TEST(Formatting, ToStringHeader) {
        auto err = hinder::error("file_error");
        auto str = hinder::to_string(err);
        EXPECT_TRUE(str.starts_with("file_error @"));
    }

    TEST(Formatting, ToStringContainsKeyValuePairs) {
        auto err = hinder::error("file_error").message("not found").with("path", "/etc/missing");
        auto str = hinder::to_string(err);
        EXPECT_NE(str.find("message: not found"), std::string::npos);
        EXPECT_NE(str.find("path: /etc/missing"), std::string::npos);
    }

    TEST(Formatting, ToStringFlagKeyHasNoValue) {
        auto err = hinder::error().with("timed_out");
        auto str = hinder::to_string(err);
        EXPECT_NE(str.find("timed_out"), std::string::npos);
        EXPECT_EQ(str.find("timed_out:"), std::string::npos);
    }

    // ========================================================================
    // to_json formatting
    // ========================================================================

    TEST(Formatting, ToJsonContainsType) {
        auto err  = hinder::error("db_error");
        auto json = hinder::to_json(err);
        EXPECT_NE(json.find("\"type\":\"db_error\""), std::string::npos);
    }

    TEST(Formatting, ToJsonContainsSource) {
        auto err  = hinder::error("test");
        auto json = hinder::to_json(err);
        EXPECT_NE(json.find("\"source\""), std::string::npos);
        EXPECT_NE(json.find("\"file\""), std::string::npos);
        EXPECT_NE(json.find("\"line\""), std::string::npos);
    }

    TEST(Formatting, ToJsonContainsData) {
        auto err  = hinder::error("test").with("code", std::int64_t {42});
        auto json = hinder::to_json(err);
        EXPECT_NE(json.find("\"data\""), std::string::npos);
        EXPECT_NE(json.find("\"code\":42"), std::string::npos);
    }

    TEST(Formatting, ToJsonNoDataWhenEmpty) {
        auto err  = hinder::error("test");
        auto json = hinder::to_json(err);
        EXPECT_EQ(json.find("\"data\""), std::string::npos);
    }

    TEST(Formatting, ToJsonEscapesStrings) {
        auto err  = hinder::error("test").with("msg", "say \"hello\"");
        auto json = hinder::to_json(err);
        EXPECT_NE(json.find("\\\"hello\\\""), std::string::npos);
    }

    // ========================================================================
    // HINDER_FAIL macro
    // ========================================================================

    TEST(HinderFail, SimpleCase) {
        auto result = []() -> std::expected<int, hinder::error> {
            return HINDER_FAIL("cmd_error", "failed: {}", "ls");
        }();
        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().type_name(), "cmd_error");
        EXPECT_EQ(result.error().get_as<std::string>("message"), "failed: ls");
    }

    TEST(HinderFail, NoFormatArgs) {
        auto result = []() -> std::expected<int, hinder::error> {
            return HINDER_FAIL("parse_error", "unexpected token");
        }();
        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().get_as<std::string>("message"), "unexpected token");
    }

    TEST(HinderFail, ChainedWithCall) {
        auto result = []() -> std::expected<int, hinder::error> {
            return HINDER_FAIL("cmd_error", "failed: {}", "ls").with("exit_code", 1);
        }();
        ASSERT_FALSE(result.has_value());
        EXPECT_TRUE(result.error().contains("exit_code"));
    }

    TEST(HinderFail, CapturesSourceLocation) {
        auto result = []() -> std::expected<int, hinder::error> {
            return HINDER_FAIL("test_error", "oops");
        }();
        EXPECT_GT(result.error().location().line(), 0U);
    }

}  // namespace
