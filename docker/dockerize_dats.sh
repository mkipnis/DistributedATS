#!/bin/sh

set -x 

# Core
docker build -t ghcr.io/mkipnis/distributed_ats_deps:latest -f Docker.Build_DATS_Deps .
docker build -t ghcr.io/mkipnis/distributed_ats:latest -f Docker.Build_Distributed_ATS .

# Exchanges
docker build --no-cache -t ghcr.io/mkipnis/dats_crypto_clob:latest -f Docker.Crypto_CLOB .
docker build --no-cache -t ghcr.io/mkipnis/dats_ust_clob:latest -f Docker.UST_CLOB .
docker build --no-cache -t ghcr.io/mkipnis/multi_matching_engine_ats:latest -f Docker.MultiMatchingEngineATS .

# FIX WS Proxy
docker build --no-cache -t ghcr.io/mkipnis/fix_ws_proxy:latest -f Docker.fix_ws_proxy .

# Webtrader Front-end
rm -rf WebTrader
mkdir WebTrader
cp -r ../MiscClients/cpp_ws_reactjs/webtrader_reactjs_ws/package.json WebTrader
cp -r ../MiscClients/cpp_ws_reactjs/webtrader_reactjs_ws/src WebTrader
cp -r ../MiscClients/cpp_ws_reactjs/webtrader_reactjs_ws/public WebTrader
docker build -t ghcr.io/mkipnis/distributed_ats_webtrader:latest -f Docker.WebTrader .

# Push images to the github
docker push ghcr.io/mkipnis/distributed_ats_deps:latest
docker push ghcr.io/mkipnis/distributed_ats:latest
docker push ghcr.io/mkipnis/dats_crypto_clob:latest
docker push ghcr.io/mkipnis/dats_ust_clob:latest
docker push ghcr.io/mkipnis/fix_ws_proxy:latest
docker push ghcr.io/mkipnis/distributed_ats_webtrader:latest
