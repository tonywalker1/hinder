#!/usr/bin/env bash
# Build RPM and DEB packages for hinder.
#
# Usage: build-packages.sh [BUILD_DIR]
#   BUILD_DIR  Optional build directory (default: build/packages)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${1:-${REPO_DIR}/build/packages}"

echo "==> Configuring (BUILD_DIR=${BUILD_DIR})"
cmake -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DHINDER_WITH_TESTS=OFF \
    "${REPO_DIR}"

echo "==> Building"
cmake --build "${BUILD_DIR}" -j"$(nproc)"

echo "==> Generating RPM"
(cd "${BUILD_DIR}" && cpack -G RPM)

echo "==> Generating DEB"
(cd "${BUILD_DIR}" && cpack -G DEB)

echo ""
echo "==> Packages written to: ${BUILD_DIR}"
find "${BUILD_DIR}" \( -name "*.rpm" -o -name "*.deb" \) | sort | sed 's/^/    /'
