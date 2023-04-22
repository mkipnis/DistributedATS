#!/bin/bash

set -x

CURRENT_DIR=`pwd`

QUICKFIX_PKG=1.15.1
LIQUIBOOK_PKG=2.0.0

[[ ! -f quickfix-v$QUICKFIX_PKG.tar.gz ]] && curl -L "https://github.com/quickfix/quickfix/archive/refs/tags/v$QUICKFIX_PKG.tar.gz"  -o quickfix-v$QUICKFIX_PKG.tar.gz
[[ ! -f liquibook-$LIQUIBOOK_PKG.tar.gz ]] && curl -L "https://github.com/enewhuis/liquibook/archive/refs/tags/$LIQUIBOOK_PKG.tar.gz"  -o liquibook-$LIQUIBOOK_PKG.tar.gz

if [[ ! -f /include/quickfix/config-all.h ]]
then

[[ ! -d quickfix-$QUICKFIX_PKG ]] && tar xvf quickfix-v$QUICKFIX_PKG.tar.gz
cd quickfix-$QUICKFIX_PKG
./bootstrap
./configure
make install
fi

if [[ ! -f $INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src/liquibook_export.h ]]
then

cd $CURRENT_DIR
[[ ! -d liquibook-$LIQUIBOOK_PKG ]] && tar xvf liquibook-$LIQUIBOOK_PKG.tar.gz

fi

ln -sf /usr/lib/x86_64-linux-gnu/libsqlite3.so /usr/lib/libsqlite3.so

[[ ! -f /opt/DistributedATS ]] && git clone https://github.com/mkipnis/DistributedATS /opt/DistributedATS

cd /opt/DistributedATS

/bin/cat <<EOM > distributed_ats_env.sh

set -x

source /opt/OpenDDS/setenv.sh

export QUICKFIX_HOME=/usr/local
export DATS_HOME=/opt/DistributedATS
export LIQUIDBOOK_HOME=$CURRENT_DIR/liquibook-$LIQUIBOOK_PKG
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$DATS_HOME/lib:

EOM

exit 0
