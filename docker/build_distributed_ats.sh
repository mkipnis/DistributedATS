#!/bin/bash

git clone -b master https://github.com/ianngulube/DistributedATS /opt/distributed_ats_src

cd /opt/distributed_ats_src
cmake -S . -B build -DCMAKE_CXX_STANDARD=17 -DPQXX_INCLUDE_DIR=/usr/local/include -DPQXX_LIBRARY_DIR=/usr/local/lib -DCMAKE_INSTALL_PREFIX=/usr/local -DDDS_ROOT_DIR=/usr/local -DLOG4CXX_ROOT_DIR=/usr/local -DQUICKFIX_ROOT_DIR=/usr/local -DLIQUIBOOK_ROOT_DIR=/usr/local
cmake --build build --verbose --target install
