#!/bin/bash

git clone -b enhancements_0424 https://github.com/mkipnis/DistributedATS /opt/distributed_ats_src

cd /opt/distributed_ats_src
HOME=/opt ./build_with_cmake.sh
