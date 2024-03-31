#!/bin/bash

set -x 

DATS_SOURCE_DIR=`pwd`
DEPS_BUILD_DIR=/opt/distributed_ats_deps_build

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
QUICKFIX_PKG=1.15.1
LIQUIBOOK_PKG=2.0.0
LOG4CXX_PKG=1.2.0

[[ ! -f $DEPS_BUILD_DIR/memory-v$FOONATHAN_MEMORY_PKG.tar.gz ]] && curl -L "https://github.com/foonathan/memory/archive/refs/tags/v$FOONATHAN_MEMORY_PKG.tar.gz"  -o $DEPS_BUILD_DIR/memory-v$FOONATHAN_MEMORY_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/Fast-CDR-v$FAST_CDR_PKG.tar.gz ]] && curl -L "https://github.com/eProsima/Fast-CDR/archive/refs/tags/v$FAST_CDR_PKG.tar.gz"  -o $DEPS_BUILD_DIR/Fast-CDR-v$FAST_CDR_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/Fast-DDS-v$FAST_DDS_PKG.tar.gz ]] && curl -L "https://github.com/eProsima/Fast-DDS/archive/refs/tags/v$FAST_DDS_PKG.tar.gz"  -o $DEPS_BUILD_DIR/Fast-DDS-v$FAST_DDS_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz ]] && curl -L "https://github.com/quickfix/quickfix/archive/refs/tags/v$QUICKFIX_PKG.tar.gz"  -o $DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/log4cxx-$LOG4CXX_PKG.tar.gz ]] && curl -L "https://github.com/apache/logging-log4cxx/archive/refs/tags/rel/v$LOG4CXX_PKG.tar.gz"  -o $DEPS_BUILD_DIR/log4cxx-$LOG4CXX_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz ]] && curl -L "https://github.com/enewhuis/liquibook/archive/refs/tags/$LIQUIBOOK_PKG.tar.gz"  -o $DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz

export INSTALL_PREFIX=$INSTALL_DIR

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
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON -DSHM_TRANSPORT_DEFAULT=OFF -DCOMPILE_EXAMPLES=ON -DINSTALL_EXAMPLES=ON
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/quickfix/config-all.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d quickfix-$QUICKFIX_PKG ]] && tar xvf quickfix-v$QUICKFIX_PKG.tar.gz
cd quickfix-$QUICKFIX_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS="-std=c++11"
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/include/log4cxx/log4cxx.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d $LOG4CXX ]] && tar xvf log4cxx-$LOG4CXX_PKG.tar.gz
cd logging-log4cxx-rel-v$LOG4CXX_PKG
mkdir build; cd build;
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=false
cmake --build . --target install
fi

if [[ ! -f $INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src/liquibook_export.h ]]
then
cd $DEPS_BUILD_DIR
[[ ! -d liquibook-$LIQUIBOOK_PKG ]] && tar xvf liquibook-$LIQUIBOOK_PKG.tar.gz
ln -s liquibook-$LIQUIBOOK_PKG liquibook
fi

mkdir /opt/DistributedATS

/bin/cat <<EOM > /opt/DistributedATS/dats_env.sh

export DATS_HOME=/opt/DistributedATS
export DEPS_HOME=/opt/distributed_ats_deps_build

export LD_LIBRARY_PATH=$DEPS_HOME/lib:$DATS_HOME/lib:$LD_LIBRARY_PATH
export LOG4CXX_CONFIGURATION=$DATS_HOME/config/log4cxx.xml

export BASEDIR_ATS=$DATS_HOME/MiscATS/CryptoCLOB
#export BASEDIR_ATS=$DATS_HOME/MiscATS/USTreasuryCLOB
#export BASEDIR_ATS=$DATS_HOME/MiscATS/MultiMatchingEngineATS
export DATS_LOG_HOME=$BASEDIR_ATS/logs

EOM
