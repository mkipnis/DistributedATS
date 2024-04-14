#!/bin/bash

trap cleanup 1 2 3 6

cleanup()
{
	echo "Caught Signal ... cleaning up..."

	$BASEDIR_ATS/scripts/stop_ats.sh

	echo "Done."

	exit 1;
}

sleep 1
$DATS_HOME/scripts/dataservice.sh start data_service_a.ini

sleep 1
$DATS_HOME/scripts/dataservice.sh start data_service_b.ini

sleep 1
$DATS_HOME/scripts/matchingengine.sh start matching_engine_MARKET_UST.ini

sleep 1
$DATS_HOME/scripts/fixgateway.sh start fix_gwy_1.cfg

sleep 1
$DATS_HOME/scripts/fixgateway.sh start fix_gwy_2.cfg

while true; do 
	$DATS_HOME/scripts/dataservice.sh check data_service_a.ini
	$DATS_HOME/scripts/dataservice.sh check data_service_b.ini
	$DATS_HOME/scripts/matchingengine.sh check matching_engine_MARKET_UST.ini
	$DATS_HOME/scripts/fixgateway.sh check fix_gwy_1.cfg
	$DATS_HOME/scripts/fixgateway.sh check fix_gwy_2.cfg

	sleep 2;
done
