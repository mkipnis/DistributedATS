#!/bin/bash

set -x

cd /opt/DistributedATS

. ./distributed_ats_env.sh

mwc.pl -type gnuace
make
