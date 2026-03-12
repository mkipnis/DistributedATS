#!/bin/sh

set -x 

# Core
docker build --no-cache -t ianngulube/distributed_ats_deps:latest -f Docker.Build_DATS_Deps .
docker build --no-cache -t ianngulube/distributed_ats:latest -f Docker.Build_Distributed_ATS .

# Exchanges
docker build --no-cache -t ianngulube/dats_crypto_clob:latest -f Docker.Crypto_CLOB .
docker build --no-cache -t ianngulube/dats_ust_clob:latest -f Docker.UST_CLOB .
docker build --no-cache -t ianngulube/multi_matching_engine_ats:latest -f Docker.MultiMatchingEngineATS .

# FIX WS Proxy
docker build --no-cache -t ianngulube/fix_ws_proxy:latest -f Docker.fix_ws_proxy .

# Webtrader Front-end
rm -rf WebTrader
mkdir WebTrader
cp -r ../MiscClients/cpp_ws_reactjs/webtrader_reactjs_ws/package.json WebTrader
cp -r ../MiscClients/cpp_ws_reactjs/webtrader_reactjs_ws/src WebTrader
cp -r ../MiscClients/cpp_ws_reactjs/webtrader_reactjs_ws/public WebTrader
docker build -t ianngulube/distributed_ats_webtrader:latest -f Docker.WebTrader .

# Push images to the github
docker push docker.io/ianngulube/distributed_ats_deps:latest
docker push docker.io/ianngulube/distributed_ats:latest
docker push docker.io/ianngulube/dats_crypto_clob:latest
docker push docker.io/ianngulube/dats_ust_clob:latest
docker push docker.io/ianngulube/fix_ws_proxy:latest
docker push docker.io/ianngulube/distributed_ats_webtrader:latest
