//
// hinder::exception
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
#include <hinder/exception/exception.h>

using namespace Catch::Matchers;
using namespace hinder;

SCENARIO("defining a new exception type", "[exception]") {
    HINDER_DEFINE_EXCEPTION(something, generic_error);
    WHEN("when throwing the exception") {
        THEN("it can be caught by its type or base types") {
            CHECK_THROWS_AS(throw something {""}, something);
            CHECK_THROWS_AS(throw something {""}, generic_error);
            CHECK_THROWS_AS(throw something {""}, std::runtime_error);
            CHECK_THROWS_AS(throw something {""}, std::exception);
        }
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(throw something {"the answer is 42"}, "the answer is 42");
        }
    }
}

SCENARIO("using the throw macro", "[exception]") {

    WHEN("throwing a generic_error exception") {
        THEN("the exception should be thrown") {
            CHECK_THROWS_AS(HINDER_THROW(generic_error, "the answer is 42"), generic_error);
        }
    }

#ifdef HINDER_WITH_EXCEPTION_SOURCE

    exception_config::format = message_format::DEFAULT;
    WHEN("using default formatting with source location") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches("generic_error: the answer is 42 "
                                      "@.*/tests/exception/exception_tests.cpp:\\d+"));
        }
    }

    exception_config::format = message_format::USER;
    WHEN("using user formatting with source location") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "{0} - {1}, {2} the answer is {3}", 42),
                              Matches("generic_error - .*/tests/exception/exception_tests.cpp, "
                                      "\\d+ the answer is 42"));
        }
    }

    exception_config::format = message_format::STRUCTURED;
    WHEN("using structured formatting with source location") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(R"(^\{\"\w.*)"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(R"(.*\d\}\}$)"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(".*\"message time\": "
                                      "\"\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{9}Z\".*"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Contains("\"message type\": \"exception\""));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Contains("\"exception type\": \"generic_error\""));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Contains("\"message\": \"the answer is 42\""));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(".*\"source\": \\{\"file\": .*, \"line\": .*\\}.*"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(".*\"file\": \".*/tests/exception/exception_tests.cpp\".*"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(".*\"line\": \\d{1,3}.*"));
        }
    }

#else

    exception_config::format = message_format::DEFAULT;
    WHEN("using default formatting without source location") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Equals("generic_error: the answer is 42"));
        }
    }

    exception_config::format = message_format::USER;
    WHEN("using user formatting without source location") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "{0} - the answer is {1}", 42),
                              Equals("generic_error - the answer is 42"));
        }
    }

    exception_config::format = message_format::STRUCTURED;
    WHEN("using structured formatting without source location") {
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(R"(^\{\"\w.*)"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(R"(.*\"\}$)"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Matches(".*\"message time\": "
                                      "\"\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{9}Z\".*"));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Contains("\"message type\": \"exception\""));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Contains("\"exception type\": \"generic_error\""));
            CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                              Contains("\"message\": \"the answer is 42\""));
        }
    }

#endif
}

SCENARIO("using the assertion macros", "[exception]") {
    WHEN("the condition is false") {
        THEN("the exception should be thrown") {
            CHECK_THROWS_AS(HINDER_INVARIANT(0 == 1, generic_error, "the answer is 42"),
                            generic_error);
            CHECK_THROWS_AS(HINDER_EXPECTS(0 == 1, generic_error, "the answer is 42"),
                            generic_error);
            CHECK_THROWS_AS(HINDER_ENSURES(0 == 1, generic_error, "the answer is 42"),
                            generic_error);
        }
        THEN("the message should be returned") {
            CHECK_THROWS_WITH(HINDER_INVARIANT(0 == 1, generic_error, "the answer is 42"),
                              Contains("the answer is 42"));
            CHECK_THROWS_WITH(HINDER_EXPECTS(0 == 1, generic_error, "the answer is 42"),
                              Contains("the answer is 42"));
            CHECK_THROWS_WITH(HINDER_ENSURES(0 == 1, generic_error, "the answer is 42"),
                              Contains("the answer is 42"));
        }
    }
    WHEN("the condition is true") {
        THEN("the exception should not be thrown") {
            CHECK_NOTHROW(HINDER_INVARIANT(true, generic_error, "the answer is 42"));
            CHECK_NOTHROW(HINDER_EXPECTS(true, generic_error, "the answer is 42"));
            CHECK_NOTHROW(HINDER_ENSURES(true, generic_error, "the answer is 42"));
        }
    }
}
