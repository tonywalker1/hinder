//
// hinder::assert
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

#include <catch2/catch.hpp>
#include <hinder/assert/assert.h>
#include <hinder/assert/handlers/throw.h>
#include <hinder/exception/exception.h>

using namespace Catch::Matchers;
using namespace hinder;

SCENARIO("setup", "[assert]") {
    assert_config::handler = std::unique_ptr<assert_handler_base>(new throw_assert_handler);
}

SCENARIO("using the assert macro", "[assert]") {

    WHEN("the assert condition is false") {
        THEN("an exception should be thrown") {
            CHECK_THROWS_AS(HINDER_ASSERT(1 == 0, "the answer is 42"), assertion_error);
        }
    }

    assert_config::format = message_format::DEFAULT;
    WHEN("using default formatting") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Matches("Assertion '1 == 0' failed: the answer is 42 "
                                      "@.*/tests/assert/assert_tests.cpp:\\d+"));
        }
    }

    assert_config::format = message_format::USER;
    WHEN("using user formatting") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(
                HINDER_ASSERT(1 == 0, "the answer is {3} [{0}, {1}, {2}]", 42),
                Matches(R"(the answer is 42 \[1 == 0, .*/tests/assert/assert_tests.cpp, \d+])"));
        }
    }

    assert_config::format = message_format::STRUCTURED;
    WHEN("using structured formatting") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"), Matches(R"(^\{\"\w.*)"));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"), Matches(R"(.*\d\}\}$)"));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Matches(".*\"message time\": "
                                      "\"\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{9}Z\".*"));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Contains("\"message type\": \"assertion failure\""));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Contains("\"assertion condition\": \"1 == 0\""));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Contains("\"message\": \"the answer is 42\""));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Matches(".*\"source\": \\{\"file\": .*, \"line\": .*\\}.*"));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Matches(".*\"file\": \".*/tests/assert/assert_tests.cpp\".*"));
            CHECK_THROWS_WITH(HINDER_ASSERT(1 == 0, "the answer is 42"),
                              Matches(".*\"line\": \\d{2}.*"));
        }
    }
}
