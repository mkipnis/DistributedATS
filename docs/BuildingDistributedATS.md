## Building Distributed ATS

All DistributedATS dependences need to be built prior to building the project.

Upon successfully building and installing all the dependences, the following set of commands can be used to build the project:
```
mkdir build
cd build

cmake .. -Dfastcdr_DIR=$INSTALL_DIR/lib/cmake/fastcdr/ -Dfastrtps_DIR=$INSTALL_DIR/share/fastrtps/cmake/ -Dfoonathan_memory_DIR=$INSTALL_DIR/lib/foonathan_memory/cmake/ -Dlog4cxx_DIR=$INSTALL_DIR/lib/cmake/log4cxx -DCMAKE_INSTALL_PREFIX=$HOME/DistributedATS -DBoost_INCLUDE_DIR=$INSTALL_DIR/include -DLIQUIBOOK_HOME=$INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src -DQUICKFIX_INSTALL_PREFIX=$INSTALL_DIR

cmake --build . --target install --config Debug
```
