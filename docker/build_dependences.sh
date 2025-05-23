#!/bin/bash

set -x

# Default to system prefix if not specified by user
DEFAULT_INSTALL_DIR="/usr/local"
INSTALL_DIR="${1:-$DEFAULT_INSTALL_DIR}"

build_and_install() {
    local repo="$1"
    local branch="$2"

    git clone -b "$branch" "$repo"

    cd "$(basename "$repo" .git)" || exit 1

    mkdir -p build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
    cmake --build . --target install

    cd ../.. || exit 1
}

build_and_install "https://github.com/foonathan/memory.git" "main"
build_and_install "https://github.com/eProsima/Fast-CDR.git" "master"
build_and_install "https://github.com/eProsima/Fast-DDS.git" "master"
build_and_install "https://github.com/apache/logging-log4cxx.git" "master"
build_and_install "https://github.com/quickfix/quickfix.git" "master"
build_and_install "https://github.com/mkipnis/liquibook.git" "cmake"
