#!/bin/sh

set -x 

# Core
#docker build -t distributed_ats_fast_dds -f Docker.build_distributed_ats .
docker build -t ghcr.io/mkipnis/distributed_ats_fast_dds:latest -f Docker.build_distributed_ats .
docker push ghcr.io/mkipnis/distributed_ats_fast_dds:latest

# Front-end
DOCKER_DIR=`pwd`
ln -sf ../MiscClients/spring_reactjs/ WebTrader
cp fixclient_docker.cfg WebTrader/src/main/resources/fixclient.cfg
cd WebTrader
mvn clean
mvn install
cp target/WebTraderRest-0.0.1-SNAPSHOT.war $DOCKER_DIR
cd $DOCKER_DIR

#docker build -t distributed_ats_webtrader -f Docker.WebTrader .
docker build -t ghcr.io/mkipnis/distributed_ats_webtrader:latest -f Docker.WebTrader .
docker push ghcr.io/mkipnis/distributed_ats_webtrader:latest
