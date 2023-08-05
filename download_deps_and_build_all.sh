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

OPENDDS_PKG=OpenDDS-3.22
QUICKFIX_PKG=1.15.1
SQLITE_PKG=3.39.4
LIQUIBOOK_PKG=2.0.0

[[ ! -f $DEPS_BUILD_DIR/$OPENDDS_PKG.tar.gz ]] && curl -L "https://github.com/objectcomputing/OpenDDS/releases/download/DDS-3.22/$OPENDDS_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$OPENDDS_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz ]] && curl -L "https://github.com/quickfix/quickfix/archive/refs/tags/v$QUICKFIX_PKG.tar.gz"  -o $DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/sqlite-$SQLITE_PKG.tar.gz ]] && curl -L "https://github.com/sqlite/sqlite/archive/refs/tags/version-$SQLITE_PKG.tar.gz"  -o $DEPS_BUILD_DIR/sqlite-$SQLITE_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz ]] && curl -L "https://github.com/enewhuis/liquibook/archive/refs/tags/$LIQUIBOOK_PKG.tar.gz"  -o $DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz

export INSTALL_PREFIX=$INSTALL_DIR

if [[ ! -f $INSTALL_DIR/include/dds/Version.h ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d $OPENDDS_PKG ]] && tar xvf $OPENDDS_PKG.tar.gz
cd $OPENDDS_PKG
./configure --std=c++11
make install -j 10

fi


if [[ ! -f $INSTALL_DIR/include/quickfix/config-all.h ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d quickfix-$QUICKFIX_PKG ]] && tar xvf quickfix-v$QUICKFIX_PKG.tar.gz
cd quickfix-$QUICKFIX_PKG
./bootstrap
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR
make install -j 10
fi

if [[ ! -f $INSTALL_DIR/include/sqlite3.h ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d sqlite-$SQLITE_PKG ]] && tar xvf sqlite-$SQLITE_PKG.tar.gz
cd sqlite-version-$SQLITE_PKG
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR
make install -j 10

fi

if [[ ! -f $INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src/liquibook_export.h ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d liquibook-$LIQUIBOOK_PKG ]] && tar xvf liquibook-$LIQUIBOOK_PKG.tar.gz

fi


cd $DATS_SOURCE_DIR

/bin/cat <<EOM > distributed_ats_env.sh

. $INSTALL_DIR/$OPENDDS_PKG/setenv.sh

export QUICKFIX_HOME=$INSTALL_DIR
export LIQUIDBOOK_HOME=$INSTALL_DIR/liquibook-$LIQUIBOOK_PKG
export SQLITE_HOME=$INSTALL_DIR
export DATS_HOME=$DATS_SOURCE_DIR
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DATS_HOME/lib:$QUICKFIX_HOME/lib

EOM

. ./distributed_ats_env.sh
$ACE_ROOT/bin/mwc.pl -type gnuace
make realclean
make 

cd DataService/sql/sqlite 
./create_database.sh

cd $DATS_SOURCE_DIR

cd MiscATS/MultiMatchingEngineATS/sql/
./populate_db.sh

cd $DATS_SOURCE_DIR

#cd MiscATS/MultiMatchingEngineATS/scripts
#./start_ats.sh

#cd $DATS_SOURCE_DIR

#cd MiscATS/MultiMatchingEngineATS/scripts
#./stop_ats.sh

#cd $DATS_SOURCE_DIR

