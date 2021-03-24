#!/usr/bin/env bash

# MIT License
#
# Copyright (c) 2019-2021 Tony Walker
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

BUILD_ROOT="/build/debian-11"
SRC_DIR="/hinder"

build_config()
{
    COMPILER="$1"
    CPP_VER="$2"
    BUILD_DIR="$BUILD_ROOT/$COMPILER-cpp$CPP_VER"

    mkdir -p "$BUILD_DIR"
    pushd "$BUILD_DIR"
    echo "--> $(pwd)"

    # set the compiler
    # set the compiler
    if [ "$COMPILER" = "clang" ]; then
        echo "* Setting the compiler to Clang."
        CXX_CMD="clang++;"
    else
        echo "* Setting the compiler to GCC."
        CXX_CMD="g++;"
    fi

    cmake -DCMAKE_CXX_COMPILER="$CXX_CMD" \
            -DCMAKE_CXX_STANDARD="$CPP_VER" \
            -DHINDER_WITH_TESTS=ON \
            -DHINDER_WITH_EXCEPTION_SOURCE=ON \
            -S "$SRC_DIR" \
            -B "$BUILD_DIR" \
        && make -j \
        && ctest
    if [ $? ]; then
        popd
        echo "<-- $(pwd)"
    else
        popd
        echo "<-- $(pwd)"
        exit 1
    fi
}

if [ -f /etc/hinder_container.conf ]; then
    echo "* Rebuilding all in $BUILD_DIR"
else
    echo "*** ERROR: You should only run this from inside the container!!!"
    exit 1
fi

# remove all previous builds
echo "Deleting previous builds from $BUILD_ROOT"
rm -rf "$BUILD_ROOT"

# now, rebuild all
build_config "gcc" "11"
build_config "gcc" "14"
build_config "gcc" "17"
build_config "gcc" "20"
build_config "clang" "11"
build_config "clang" "14"
build_config "clang" "17"
build_config "clang" "20"
