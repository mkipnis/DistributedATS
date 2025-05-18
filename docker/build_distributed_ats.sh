#!/bin/bash

[[ ! -f /opt/distributed_ats_src ]] && git enhancements_0424 -b master https://github.com/mkipnis/DistributedATS /opt/distributed_ats_src

cd /opt/distributed_ats_src
HOME=/opt ./build_with_cmake.sh
