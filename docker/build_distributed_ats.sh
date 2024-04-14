#!/bin/bash

[[ ! -f /opt/distributed_ats_src ]] && git clone -b FastDDS https://github.com/mkipnis/DistributedATS /opt/distributed_ats_src

cd /opt/distributed_ats_src
rm -rf build
mkdir build
cd build

export DEPS_INSTALL_DIR=/opt/distributed_ats_deps_build/
cmake .. -Dfastcdr_DIR=$DEPS_INSTALL_DIR/lib/cmake/fastcdr/ -Dfastrtps_DIR=$DEPS_INSTALL_DIR/share/fastrtps/cmake/ -Dfoonathan_memory_DIR=$DEPS_INSTALL_DIR/lib/foonathan_memory/cmake/ -Dlog4cxx_DIR=$DEPS_INSTALL_DIR/lib/cmake/log4cxx -DCMAKE_INSTALL_PREFIX=/opt/DistributedATS -DLIQUIBOOK_HOME=$DEPS_INSTALL_DIR/liquibook/src -DQUICKFIX_INSTALL_PREFIX=$DEPS_INSTALL_DIR

cmake --build . --target install --config Debug

chmod +x /opt/DistributedATS/scripts/*.sh
chmod +x /opt/DistributedATS/MiscATS/MultiMatchingEngineATS/scripts/*.sh
chmod +x /opt/DistributedATS/MiscATS/USTreasuryCLOB/scripts/*.sh
chmod +x /opt/DistributedATS/MiscATS/CryptoCLOB/scripts/*.sh
