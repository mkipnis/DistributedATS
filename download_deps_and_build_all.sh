#!/bin/bash

set -x 

DATS_SOURCE_DIR=`pwd`
DEPS_BUILD_DIR=$HOME/distributed_ats_deps_build

mkdir -p $DEPS_BUILD_DIR 

if [[ "$OSTYPE" == "darwin"* ]]; then
	CXXFLAGS="-DWAZOO_64_BIT -std=c++11 -stdlib=libc++"
fi

if [ -z "$1" ] 
then
    INSTALL_DIR=$DEPS_BUILD_DIR
    echo "Install directory is not supplied, installing in $INSTALL_DIR/"
else
    INSTALL_DIR=$1
fi

FOONATHAN_MEMORY_PKG=0.7-3
FAST_CDR_PKG=2.2.0
FAST_DDS_PKG=2.13.3
BOOST_PKG=boost_1_80_0
QUICKFIX_PKG=1.15.1
SQLITE_PKG=release
TINYXML2_PKG=10.0.0
ASIO_PKG=1.28.1
LIQUIBOOK_PKG=2.0.0
LOG4CXX_PKG=1.2.0
APR_PKG=1.7.4
APR_UTIL_PKG=1.6.3

[[ ! -f $DEPS_BUILD_DIR/$BOOST_PKG.tar.gz ]] && curl -L "https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/$BOOST_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$BOOST_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/asio-$ASIO_PKG.tar.gz ]] && curl -L "https://sourceforge.net/projects/asio/files/asio/1.28.1%20%28Stable%29/asio-$ASIO_PKG.tar.gz/download/"  -o $DEPS_BUILD_DIR/asio-$ASIO_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/apr-$APR_PKG.tar.gz ]] && curl -L "https://github.com/apache/apr/archive/refs/tags/$APR_PKG.tar.gz"  -o $DEPS_BUILD_DIR/apr-$APR_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/apr-util-$APR_UTIL_PKG.tar.gz ]] && curl -L "https://github.com/apache/apr-util/archive/refs/tags/$APR_UTIL_PKG.tar.gz"  -o $DEPS_BUILD_DIR/apr-util-$APR_UTIL_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/log4cxx-$LOG4CXX_PKG.tar.gz ]] && curl -L "https://github.com/apache/logging-log4cxx/archive/refs/tags/rel/v$LOG4CXX_PKG.tar.gz"  -o $DEPS_BUILD_DIR/log4cxx-$LOG4CXX_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/tinyxml2-$TINYXML2_PKG.tar.gz ]] && curl -L "https://github.com/leethomason/tinyxml2/archive/refs/tags/$TINYXML2_PKG.tar.gz"  -o $DEPS_BUILD_DIR/tinyxml2-$TINYXML2_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/memory-v$FOONATHAN_MEMORY_PKG.tar.gz ]] && curl -L "https://github.com/foonathan/memory/archive/refs/tags/v$FOONATHAN_MEMORY_PKG.tar.gz"  -o $DEPS_BUILD_DIR/memory-v$FOONATHAN_MEMORY_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/Fast-CDR-v$FAST_CDR_PKG.tar.gz ]] && curl -L "https://github.com/eProsima/Fast-CDR/archive/refs/tags/v$FAST_CDR_PKG.tar.gz"  -o $DEPS_BUILD_DIR/Fast-CDR-v$FAST_CDR_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/Fast-DDS-v$FAST_DDS_PKG.tar.gz ]] && curl -L "https://github.com/eProsima/Fast-DDS/archive/refs/tags/v$FAST_DDS_PKG.tar.gz"  -o $DEPS_BUILD_DIR/Fast-DDS-v$FAST_DDS_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz ]] && curl -L "https://github.com/quickfix/quickfix/archive/refs/tags/v$QUICKFIX_PKG.tar.gz"  -o $DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/sqlite-$SQLITE_PKG.tar.gz ]] && curl -L "https://github.com/sqlite/sqlite/archive/refs/tags/$SQLITE_PKG.tar.gz"  -o $DEPS_BUILD_DIR/sqlite-$SQLITE_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz ]] && curl -L "https://github.com/enewhuis/liquibook/archive/refs/tags/$LIQUIBOOK_PKG.tar.gz"  -o $DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz

export INSTALL_PREFIX=$INSTALL_DIR

if [[ ! -f $INSTALL_DIR/include/apr-1/apr.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $APR_PKG ]] && tar xvf apr-$APR_PKG.tar.gz
cd apr-$APR_PKG
./buildconf
./configure --prefix=$INSTALL_PREFIX --exec-prefix=$INSTALL_PREFIX
make install -j 20
fi

if [[ ! -f $INSTALL_DIR/include/apr-util-1/apr.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $APR_UTIL_PKG ]] && tar xvf apr-util-$APR_UTIL_PKG.tar.gz
cd apr-util-$APR_UTIL_PKG
./buildconf --with-apr=$INSTALL_PREFIX/apr-$APR_PKG
./configure --with-apr=$INSTALL_PREFIX/apr-$APR_PKG --prefix=$INSTALL_PREFIX --exec-prefix=$INSTALL_PREFIX
make install -j 20
fi

if [[ ! -f $INSTALL_DIR/include/tinyxml2.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d tinyxml2-$TINYXML2_PKG ]] && tar xvf tinyxml2-$TINYXML2_PKG.tar.gz
cd tinyxml2-$TINYXML2_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/boost/version.hpp ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $BOOST_PKG ]] && tar xvf $BOOST_PKG.tar.gz
cd $BOOST_PKG
./bootstrap.sh --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR
./b2 install
fi

if [[ ! -f $INSTALL_DIR/include/asio.hpp ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d asio-$TINYXML2_PKG ]] && tar xvf asio-$ASIO_PKG.tar.gz
cd asio-$ASIO_PKG
./configure --prefix=$INSTALL_PREFIX --exec-prefix=$INSTALL_PREFIX
make install -j 20
fi

if [[ ! -f $INSTALL_DIR/include/log4cxx/log4cxx.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $LOG4CXX ]] && tar xvf log4cxx-$LOG4CXX_PKG.tar.gz
cd logging-log4cxx-rel-v$LOG4CXX_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON  -DAPR_LIBRARIES=$INSTALL_PREFIX -DAPR_UTIL_LIBRARIES=$INSTALL_PREFIX -DBUILD_TESTING=false
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/foonathan_memory/foonathan/memory/config.hpp ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $FOONATHAN_MEMORY_PKG ]] && tar xvf memory-v$FOONATHAN_MEMORY_PKG.tar.gz
cd memory-$FOONATHAN_MEMORY_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/fastcdr/config.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $FAST_CDR_PKG ]] && tar xvf Fast-CDR-v$FAST_CDR_PKG.tar.gz
cd Fast-CDR-$FAST_CDR_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/fastdds/rtps/rtps_all.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $FAST_DDS_PKG ]] && tar xvf Fast-DDS-v$FAST_DDS_PKG.tar.gz
cd Fast-DDS-$FAST_DDS_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON -DAsio_INCLUDE_DIR=$INSTALL_DIR/include
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/quickfix/config-all.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d quickfix-$QUICKFIX_PKG ]] && tar xvf quickfix-v$QUICKFIX_PKG.tar.gz
cd quickfix-$QUICKFIX_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS="-std=c++0x"
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/sqlite3.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d sqlite-$SQLITE_PKG ]] && tar xvf sqlite-$SQLITE_PKG.tar.gz
cd sqlite-$SQLITE_PKG
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR
make install -j 10
fi

if [[ ! -f $INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src/liquibook_export.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d liquibook-$LIQUIBOOK_PKG ]] && tar xvf liquibook-$LIQUIBOOK_PKG.tar.gz
fi

cd $DATS_SOURCE_DIR

/bin/cat <<EOM > $HOME/DistributedATS/dats_env.sh

export DATS_HOME=$HOME/DistributedATS
export DEPS_HOME=$INSTALL_DIR

export DYLD_LIBRARY_PATH=$DEPS_HOME:$DATS_HOME/lib:$DYLD_LIBRARY_PATH
export LOG4CXX_CONFIGURATION=$DATS_HOME/config/log4cxx.xml

export BASEDIR_ATS=$DATS_HOME/MiscATS/CryptoCLOB
#export BASEDIR_ATS=$DATS_HOME/MiscATS/USTreasuryCLOB
#export BASEDIR_ATS=$DATS_HOME/MiscATS/MultiMatchingEngineATS
export DATS_LOG_HOME=$BASEDIR_ATS/logs

EOM

mkdir build
cd build

cmake -G Xcode .. -Dfastcdr_DIR=$INSTALL_DIR/lib/cmake/fastcdr/ -Dfastrtps_DIR=$INSTALL_DIR/share/fastrtps/cmake/ -Dfoonathan_memory_DIR=$INSTALL_DIR/lib/foonathan_memory/cmake/ -Dlog4cxx_DIR=$INSTALL_DIR/lib/cmake/log4cxx -DCMAKE_INSTALL_PREFIX=$HOME/DistributedATS -DBoost_INCLUDE_DIR=$INSTALL_DIR/include -DLIQUIBOOK_HOME=$INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src -DQUICKFIX_INSTALL_PREFIX=$INSTALL_DIR

#cmake .. -Dfastcdr_DIR=$INSTALL_DIR/lib/cmake/fastcdr/ -Dfastrtps_DIR=$INSTALL_DIR/share/fastrtps/cmake/ -Dfoonathan_memory_DIR=$INSTALL_DIR/lib/foonathan_memory/cmake/ -Dlog4cxx_DIR=$INSTALL_DIR/lib/cmake/log4cxx -DCMAKE_INSTALL_PREFIX=$HOME/DistributedATS -DBoost_INCLUDE_DIR=$INSTALL_DIR/include -DLIQUIBOOK_HOME=$INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src -DQUICKFIX_INSTALL_PREFIX=$INSTALL_DIR

cmake --build . --target install --config Debug
