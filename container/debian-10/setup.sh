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

# build the docker image
ROOT_DIR=/hinder
set -ex

apt-get update

apt-get install -y --no-install-recommends \
    ca-certificates \
    clang \
    clang-format \
    clang-tidy \
    clang-tools \
    cmake \
    g++ \
    gcc \
    git \
    libfmt-dev \
    make


# install Catch2
SCRATCH_DIR=/tmp/build_scratch
rm -rf $SCRATCH_DIR
mkdir -p $SCRATCH_DIR
pushd $SCRATCH_DIR

# clone
git clone --branch v2.x https://github.com/catchorg/Catch2.git

# build and install
mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release ../Catch2 \
    && make -j \
    && ctest \
    && make install

# clean-up
popd
rm -rf $SCRATCH_DIR

# clean-up
rm -rf /var/lib/apt/lists/*
rm -f  $ROOT_DIR/setup.sh
