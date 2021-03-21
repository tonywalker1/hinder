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
    THEN("the exception should be thrown") {
        CHECK_THROWS_AS(HINDER_THROW(generic_error, "the answer is 42"), generic_error);
    }
    THEN("the message should be returned") {
        CHECK_THROWS_WITH(HINDER_THROW(generic_error, "the answer is 42"),
                          StartsWith("generic_error: the answer is 42"));
    }
}

SCENARIO("using nested exceptions", "[exception]") {
    HINDER_DEFINE_EXCEPTION(inner, generic_error);
    HINDER_DEFINE_EXCEPTION(outer, generic_error);

    THEN("the nested messages should be returned") {
        auto f = []() {
            try {
                HINDER_THROW(inner, "computation error");
            } catch (const std::exception& e) {
                try {
                    HINDER_NESTED_THROW(outer, "the answer should have been 42");
                } catch (const std::exception& e) {
                    throw;
                }
            }
        };

        CHECK_THROWS_WITH(f(), StartsWith("outer: the answer should have been 42"));

        try {
            f();
        } catch (const std::exception& e) {
            CHECK_THAT(hinder::to_string(e),
                       Matches("outer: the answer should have been 42( @.*:.*)*\n"
                               "  inner: computation error( @.*:.*)*"));
        }
    }
}

SCENARIO("using the assertion macros", "[exception]") {
    THEN("the exception should be thrown") {
        CHECK_THROWS_AS(
            HINDER_INVARIANT(0 == 1, generic_error, "the answer is 42"),
            generic_error);
        CHECK_THROWS_AS(HINDER_EXPECTS(0 == 1, generic_error, "the answer is 42"), generic_error);
        CHECK_THROWS_AS(HINDER_ENSURES(0 == 1, generic_error, "the answer is 42"), generic_error);
    }
    THEN("the message should be returned") {
        CHECK_THROWS_WITH(HINDER_INVARIANT(0 == 1, generic_error, "the answer is 42"),
                          Matches("generic_error: the answer is 42( @.*:.*)*"));
        CHECK_THROWS_WITH(HINDER_EXPECTS(0 == 1, generic_error, "the answer is 42"),
                          Matches("generic_error: the answer is 42( @.*:.*)*"));
        CHECK_THROWS_WITH(HINDER_ENSURES(0 == 1, generic_error, "the answer is 42"),
                          Matches("generic_error: the answer is 42( @.*:.*)*"));
    }
    THEN("the exception should not be thrown") {
        CHECK_NOTHROW(HINDER_INVARIANT(true, generic_error, "the answer is 42"));
        CHECK_NOTHROW(HINDER_EXPECTS(true, generic_error, "the answer is 42"));
        CHECK_NOTHROW(HINDER_ENSURES(true, generic_error, "the answer is 42"));
    }
}
