#!/bin/bash

trap cleanup 1 2 3 6

cleanup()
{
	echo "Caught Signal ... cleaning up..."

	$BASEDIR/scripts/stop_ats.sh

	echo "Done."

	exit 1;
}

echo "DDS_ROOT :" $DDS_ROOT
PROGNAME=DCPSInfoRepo
PRG="$0"

PRGDIR=`dirname "$PRG"`
BASEDIR=`cd "$PRGDIR/.." > /dev/null; pwd`

PROCESS_PATTERN="$PROGNAME -ORBEndpoint iiop://localhost:12345 -DCPSDebugLevel=0 -ORBLogFile $BASEDIR/logs/$PROGNAME.log"
echo $PROCESS_PATTERN

idn=$(id -u)

echo "User ID :" $idn

if [ ! -d "$BASEDIR/logs/" ] 
then
	mkdir $BASEDIR/logs/
fi

logfile=$BASEDIR/logs/$PROGNAME.startstop.log

#ldd $DDS_ROOT/bin/$PROGNAME >> $logfile

BROKER_PID=`pgrep -f "$PROCESS_PATTERN$"`
echo $BROKER_PID >> $logfile;
if [ ! -z "$BROKER_PID" ]
then
        echo "Broker already running - $BROKER_PID";
        echo "Broker already running - $BROKER_PID" >> $logfile;
else
	echo "Starting Broker - $PROCESS_PATTERN" 

	nohup $DDS_ROOT/bin/$PROCESS_PATTERN & >> $logfile 2>&1
	sleep 1
	BROKER_PID=`pgrep -f "$PROCESS_PATTERN$" -u $idn`

	if [ -z "BROKER_PID" ]
	then
        	echo "Broker is not running - check logs";
        	echo "Broker is not running" >> $logfile;
	fi;

	echo $(date '+%Y%m%d.%H%M%S') broker-run-done : pid,$BROKER_PID >> $logfile
fi

sleep 1
$DATS_HOME/DataService/scripts/dataservice.sh start data_service_a.ini

sleep 1
$DATS_HOME/DataService/scripts/dataservice.sh start data_service_b.ini

sleep 1
$DATS_HOME/MatchingEngine/scripts/matchingengine.sh start matching_engine_MARKET_Y.ini

sleep 1
$DATS_HOME/MatchingEngine/scripts/matchingengine.sh start matching_engine_MARKET_Z.ini

sleep 1
$DATS_HOME/FIXGateway/scripts/fixgateway.sh start fix_gwy_1.cfg

sleep 1
$DATS_HOME/FIXGateway/scripts/fixgateway.sh start fix_gwy_2.cfg

sleep 1
$DATS_HOME/FIXGateway/scripts/fixgateway.sh start fix_gwy_3.cfg

while true; do 
	$DATS_HOME/DataService/scripts/dataservice.sh check data_service_a.ini
	$DATS_HOME/DataService/scripts/dataservice.sh check data_service_b.ini
	$DATS_HOME/MatchingEngine/scripts/matchingengine.sh check matching_engine_MARKET_Y.ini
	$DATS_HOME/MatchingEngine/scripts/matchingengine.sh check matching_engine_MARKET_Z.ini
	$DATS_HOME/FIXGateway/scripts/fixgateway.sh check fix_gwy_1.cfg
	$DATS_HOME/FIXGateway/scripts/fixgateway.sh check fix_gwy_2.cfg
	$DATS_HOME/FIXGateway/scripts/fixgateway.sh check fix_gwy_3.cfg

	sleep 30;
done
