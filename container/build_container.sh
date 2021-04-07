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

# detect and set path to container config
if [ -z "$1" ]; then
    echo "build_container.sh <path_to_config>"
    echo "Usage Example:"
    echo "  build_container.sh debian-10"
    exit 1
fi
NAME="$1"
DIR="$1"
VOL="hinder_build"
IMG="hinder-$NAME"

# detect and set tool (podman/docker) config
if [ -f /usr/bin/podman ]; then
    echo "* Using podman"
    TOOL=podman
elif [ -f /usr/bin/docker ]; then
    echo "* Using docker"
    TOOL=docker
else
    echo "build_container.sh error:"
    echo "  podman or docker must be installed to build containers"
fi

if cd "$DIR"; then
  echo "ERROR: $DIR doesn't exist."
  exit 1
fi

# create volume if needed
if $TOOL volume inspect "$VOL" >/dev/null 2>&1; then
    echo "* Found existing $VOL volume"
else
    echo "* Creating $VOL volume"
    $TOOL volume create "$VOL"
fi

#remove existing image if needed
#if $TOOL image exists "$IMG"; then
    echo "* Removing existing image $IMG"
    $TOOL rmi -f "$IMG"
#fi

# build image
echo "* Building the image..."
$TOOL build --tag "hinder-$NAME" --file ./Dockerfile.dev .

cd ..
