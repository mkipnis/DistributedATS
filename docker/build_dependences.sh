#!/bin/bash

set -x 

build_and_install() {

    local repo="$1"
    local branch="$2"
    
    git clone -b "$branch" "$repo"

    cd $(basename "$repo" .git)

    mkdir -p build && cd build
    cmake ..
    cmake --build . --target install
}

build_and_install "https://github.com/foonathan/memory.git" "main"
build_and_install "https://github.com/eProsima/Fast-CDR.git" "master"
build_and_install "https://github.com/eProsima/Fast-DDS.git" "master"
build_and_install "https://github.com/apache/logging-log4cxx.git" "master"
build_and_install "https://github.com/quickfix/quickfix.git" "master"
build_and_install "https://github.com/mkipnis/liquibook.git" "cmake"
