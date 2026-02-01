//
// hinder::misc
//
// MIT License
//
// Copyright (c) 2021  Tony Walker
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
#include <hinder/json.h>
#include <string>

using namespace hinder;

SCENARIO("adding JSON data to a message", "[hinder_misc_json]") {
    GIVEN("some string data") {
        std::string msg;
        auto        out = std::back_inserter(msg);
        std::string data;
        WHEN("given a char* nullptr") {
            THEN("the result is an empty string") {
                json_string_to(out, "key", static_cast<const char*>(nullptr));
                CHECK(msg == "\"key\": \"\"");
            }
            WHEN("given an empty char* string") {
                THEN("the result is an empty string")
                json_string_to(out, "key", "");
                CHECK(msg == "\"key\": \"\"");
            }
            WHEN("given an ordinary char* string") {
                THEN("the result is the string with quotes")
                json_string_to(out, "key", "some ordinary string");
                CHECK(msg == "\"key\": \"some ordinary string\"");
            }
            WHEN("given an empty std::string") {
                THEN("the result is the empty string") {
                    data = "";
                    json_string_to(out, "key", data);
                    CHECK(msg == "\"key\": \"\"");
                }
            }
            WHEN("given an ordinary std::string") {
                THEN("the result is the string with quotes") {
                    data = "some ordinary string";
                    json_string_to(out, "key", data);
                    CHECK(msg == "\"key\": \"some ordinary string\"");
                }
            }
        }
    }
#if defined(__cplusplus) && (__cplusplus >= HINDER_CPP_17)
    GIVEN("some string data as a string view") {
        std::string msg;
        auto        out = std::back_inserter(msg);
        std::string_view data;
        WHEN("given an empty std::string_view") {
            THEN("the result is the empty string") {
                data = "";
                json_string_to(out, "key", data);
                CHECK(msg == "\"key\": \"\"");
            }
        }
        WHEN("given an ordinary std::string_view") {
            THEN("the result is the string with quotes") {
                data = "some ordinary string";
                json_string_to(out, "key", data);
                CHECK(msg == "\"key\": \"some ordinary string\"");
            }
        }
    }
#endif
}

/*
SCENARIO("Appending JSON data to a string", "misc_json") {
    THEN("a char should have quotes") { CHECK(json_string_helper("key", "c") == "\"key\": \"c\""); }
    THEN("an empty string should have quotes") {
        CHECK(json_string_helper("", "") == "\"\": \"\"");
    }
    THEN("non-empty strings should have quotes") {
        CHECK(json_string_helper("", "") == "\"\": \"\"");
        CHECK(json_string_helper("key", "value") == "\"key\": \"value\"");
    }
    THEN("std::strings should have quotes") {
        CHECK(json_string_helper("", std::string("")) == "\"\": \"\"");
        CHECK(json_string_helper("key", std::string("value")) == "\"key\": \"value\"");
        CHECK(json_string_helper("key", std::string("value")) == "\"key\": \"value\"");
    }
#if defined(__cplusplus) && (__cplusplus >= HINDER_CPP_17)
    THEN("std::string_views should have quotes") {
        CHECK(json_string_helper("", std::string_view("")) == "\"\": \"\"");
        CHECK(json_string_helper("key", std::string_view("value")) == "\"key\": \"value\"");
        CHECK(json_string_helper("key", std::string_view("value")) == "\"key\": \"value\"");
    }
#endif
    THEN("numbers should not have quotes") {
        CHECK(json_string_helper("answer", static_cast<short int>(42)) == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<unsigned short int>(42))
              == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<int>(42)) == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<unsigned int>(42)) == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<long int>(42)) == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<unsigned long int>(42)) == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<long long int>(42)) == "\"answer\": 42");
        CHECK(json_string_helper("answer", static_cast<unsigned long long int>(42))
              == "\"answer\": 42");
        CHECK(json_string_helper("pi", static_cast<float>(3.1415)) == "\"pi\": 3.1415");
        CHECK(json_string_helper("pi", static_cast<double>(3.1415)) == "\"pi\": 3.1415");
        CHECK(json_string_helper("pi", static_cast<long double>(3.1415)) == "\"pi\": 3.1415");
    }
    THEN("bools should not have quotes and be written as true/false") {
        CHECK(json_string_helper("key", true) == "\"key\": true");
        CHECK(json_string_helper("key", false) == "\"key\": false");
    }
}
*/
