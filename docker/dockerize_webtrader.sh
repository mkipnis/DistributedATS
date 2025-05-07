#!/bin/sh

set -x 

# Front-end
DOCKER_DIR=`pwd`
ln -sf ../MiscClients/spring_reactjs/ WebTrader
cp fixclient_docker.cfg WebTrader/src/main/resources/fixclient.cfg
cd WebTrader
mvn clean
mvn install
cp target/WebTraderRest-0.0.1-SNAPSHOT.war $DOCKER_DIR
cd $DOCKER_DIR

docker build -t ghcr.io/mkipnis/distributed_ats_webtrader:latest -f Docker.WebTrader .
docker push ghcr.io/mkipnis/distributed_ats_webtrader:latest
