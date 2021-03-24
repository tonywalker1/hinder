#!/usr/bin/env sh

# MIT License
#
# Copyright (c) 2019 - 2021 Tony Walker
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

# set path to container config
if [ -z $1 ]; then
    echo "run_container.sh <container_name> <path_to_src>"
    echo "Usage Example:"
    echo "  run_container.sh hinder-debian-10 .."
    exit 1
fi
NAME="$1"
VOL="hinder_build"

# set path to hinder source
if [ -z $2 ]; then
    echo "run_container.sh <container_name> <path_to_src>"
    echo "Usage Example:"
    echo "  run_container.sh hinder-debian-10 .."
    exit 1
fi
SRC="$2"

# detect and set tool (podman/docker) config
if [ -n $(which podman) ]; then
    echo "* Using podman"
    TOOL=podman
elif [ -n $(which docker) ]; then
    echo "* Using docker"
    TOOL=docker
else
    echo "build_container.sh error:"
    echo "  podman or docker must be installed to build containers"
fi

# finally, build the container
$TOOL run \
    --rm \
    -ti \
    --mount type=bind,source="$SRC",target=/hinder \
    --mount type=volume,source="$VOL",target=/build \
    "hinder-$NAME"
