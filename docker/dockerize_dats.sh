#!/bin/sh

set -x 

# Core
docker build -t ghcr.io/mkipnis/distributed_ats_deps:latest -f Docker.Build_DATS_Deps .
docker build -t ghcr.io/mkipnis/distributed_ats:latest -f Docker.Build_Distributed_ATS .

# Exchanges
docker build --no-cache -t ghcr.io/mkipnis/dats_crypto_clob:latest -f Docker.Crypto_CLOB .
docker build --no-cache -t ghcr.io/mkipnis/dats_ust_clob:latest -f Docker.UST_CLOB .
docker build --no-cache -t ghcr.io/mkipnis/multi_matching_engine_ats:latest -f Docker.MultiMatchingEngineATS .

docker push ghcr.io/mkipnis/distributed_ats_deps:latest
docker push ghcr.io/mkipnis/distributed_ats:latest
docker push ghcr.io/mkipnis/dats_crypto_clob:latest
docker push ghcr.io/mkipnis/dats_ust_clob:latest
