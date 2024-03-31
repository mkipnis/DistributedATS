#!/bin/sh

set -x 

# Core
#docker build -t distributed_ats_fast_dds -f Docker.build_distributed_ats .
docker build -t ghcr.io/mkipnis/distributed_ats_fast_dds:latest -f Docker.build_distributed_ats .
docker push ghcr.io/mkipnis/distributed_ats_fast_dds:latest
