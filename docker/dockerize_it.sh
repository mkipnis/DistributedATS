#!/bin/sh

# Core
#docker build -t distributed_ats -f Docker.build_distributed_ats .
docker build -t ghcr.io/mkipnis/distributed_ats:latest -f Docker.build_distributed_ats .
docker push ghcr.io/mkipnis/distributed_ats:latest

# Front-end
#docker build -t distributed_ats_webtrader -f Docker.WebTrader .
docker build -t ghcr.io/mkipnis/distributed_ats_webtrader:latest -f Docker.WebTrader .
docker push ghcr.io/mkipnis/distributed_ats_webtrader:latest
