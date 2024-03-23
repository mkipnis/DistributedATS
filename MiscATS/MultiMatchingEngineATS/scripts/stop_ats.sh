#!/bin/sh

sleep 1
$DATS_HOME/scripts/dataservice.sh stop data_service_a.ini 

sleep 1
$DATS_HOME/scripts/dataservice.sh stop data_service_b.ini

sleep 1
$DATS_HOME/scripts/matchingengine.sh stop matching_engine_MARKET_Y.ini

sleep 1
$DATS_HOME/scripts/matchingengine.sh stop matching_engine_MARKET_Z.ini

sleep 1
$DATS_HOME/scripts/fixgateway.sh stop fix_gwy_1.cfg

sleep 1
$DATS_HOME/scripts/fixgateway.sh stop fix_gwy_2.cfg

sleep 1
$DATS_HOME/scripts/fixgateway.sh stop fix_gwy_3.cfg
