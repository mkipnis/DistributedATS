#!/bin/sh

set -x 

# Core
docker build --no-cache -t ghcr.io/mkipnis/distributed_ats:latest -f Docker.Build_Distributed_ATS .
docker build --no-cache -t ghcr.io/mkipnis/dats_crypto_clob:latest -f Docker.Crypto_CLOB .
docker build --no-cache -t ghcr.io/mkipnis/dats_ust_clob:latest -f Docker.UST_CLOB .

#docker push ghcr.io/mkipnis/distributed_ats:latest
#docker push ghcr.io/mkipnis/dats_crypto_clob:latest
#docker push ghcr.io/mkipnis/dats_ust_clob:latest
