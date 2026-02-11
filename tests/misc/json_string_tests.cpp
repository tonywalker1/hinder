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

#include <gtest/gtest.h>
#include <hinder/json.h>
#include <string>

using namespace hinder;

TEST(Json, CharPtrNullptr) {
    std::string msg;
    auto        out = std::back_inserter(msg);
    json_string_to(out, "key", static_cast<const char *>(nullptr));
    EXPECT_EQ(msg, "\"key\": \"\"");
}

TEST(Json, EmptyCharPtrString) {
    std::string msg;
    auto        out = std::back_inserter(msg);
    json_string_to(out, "key", "");
    EXPECT_EQ(msg, "\"key\": \"\"");
}

TEST(Json, OrdinaryCharPtrString) {
    std::string msg;
    auto        out = std::back_inserter(msg);
    json_string_to(out, "key", "some ordinary string");
    EXPECT_EQ(msg, "\"key\": \"some ordinary string\"");
}

TEST(Json, EmptyStdString) {
    std::string msg;
    auto        out  = std::back_inserter(msg);
    std::string data = "";
    json_string_to(out, "key", data);
    EXPECT_EQ(msg, "\"key\": \"\"");
}

TEST(Json, OrdinaryStdString) {
    std::string msg;
    auto        out  = std::back_inserter(msg);
    std::string data = "some ordinary string";
    json_string_to(out, "key", data);
    EXPECT_EQ(msg, "\"key\": \"some ordinary string\"");
}

#if defined(__cplusplus) && (__cplusplus >= HINDER_CPP_17)
TEST(Json, EmptyStdStringView) {
    std::string      msg;
    auto             out  = std::back_inserter(msg);
    std::string_view data = "";
    json_string_to(out, "key", data);
    EXPECT_EQ(msg, "\"key\": \"\"");
}

TEST(Json, OrdinaryStdStringView) {
    std::string      msg;
    auto             out  = std::back_inserter(msg);
    std::string_view data = "some ordinary string";
    json_string_to(out, "key", data);
    EXPECT_EQ(msg, "\"key\": \"some ordinary string\"");
}
#endif
