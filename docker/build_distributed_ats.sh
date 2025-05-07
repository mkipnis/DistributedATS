#!/bin/bash

[[ ! -f /opt/distributed_ats_src ]] && git clone -b master https://github.com/mkipnis/DistributedATS /opt/distributed_ats_src

cd /opt/distributed_ats_src
HOME=/opt ./download_deps_and_build_all.sh
