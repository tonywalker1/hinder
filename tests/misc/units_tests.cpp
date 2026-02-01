//
// hinder::misc
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
#include <hinder/units.h>

using namespace hinder;

SCENARIO("specifying a memory block", "[misc_units]") {
    THEN("the block size should be correct") {
        // trivial, but let's check anyway
        size_t block = 1;
        CHECK(KiB(1) == block * 1024);
        CHECK(MiB(1) == block * 1024 * 1024);
        CHECK(GiB(1) == block * 1024 * 1024 * 1024);
        CHECK(TiB(1) == block * 1024 * 1024 * 1024 * 1024);

        block = 4;
        CHECK(KiB(4) == block * 1024);
        CHECK(MiB(4) == block * 1024 * 1024);
        CHECK(GiB(4) == block * 1024 * 1024 * 1024);
        CHECK(TiB(4) == block * 1024 * 1024 * 1024 * 1024);

        CHECK(MiB(4) == KiB(4) * 1024);
        CHECK(GiB(4) == MiB(4) * 1024);
        CHECK(TiB(4) == GiB(4) * 1024);
    }
}
