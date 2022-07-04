#!/bin/sh

echo "DDS_ROOT :" $DDS_ROOT
PROGNAME=DCPSInfoRepo
PRG="$0"

PRGDIR=`dirname "$PRG"`
BASEDIR=`cd "$PRGDIR/.." > /dev/null; pwd`

PROCESS_PATTERN="$PROGNAME -ORBEndpoint iiop://localhost:12345 -DCPSDebugLevel=0 -ORBLogFile $BASEDIR/logs/$PROGNAME.log"

idn=$(id -u)

echo "User ID :" $idn$a

if [ ! -d "$BASEDIR/logs/" ] 
then
	mkdir $BASEDIR/logs/
fi

logfile=$BASEDIR/logs/$PROGNAME.startstop.log

#ldd $DDS_ROOT/bin/$PROGNAME >> $logfile

BROKER_PID=`pgrep -f "$PROCESS_PATTERN$" -u $idn`
echo $BROKER_PID >> $logfile;
if [ ! -z "$BROKER_PID" ]
then
        echo "Killing broker - $BROKER_PID" >> $logfile;
	pkill -SIGTERM -f "$PROCESS_PATTERN$"
fi

sleep 1
$DATS_HOME/DataService/scripts/dataservice.sh stop data_service_a.ini 

sleep 1
$DATS_HOME/DataService/scripts/dataservice.sh stop data_service_b.ini

sleep 1
$DATS_HOME/MatchingEngine/scripts/matchingengine.sh stop matching_engine_MARKET_BTC.ini

sleep 1
$DATS_HOME/MatchingEngine/scripts/matchingengine.sh stop matching_engine_MARKET_ETH.ini

sleep 1
$DATS_HOME/MatchingEngine/scripts/matchingengine.sh stop matching_engine_MARKET_OTHER_COIN.ini

sleep 1
$DATS_HOME/FIXGateway/scripts/fixgateway.sh stop fix_gwy_1.cfg

sleep 1
$DATS_HOME/FIXGateway/scripts/fixgateway.sh stop fix_gwy_2.cfg

sleep 1
$DATS_HOME/FIXGateway/scripts/fixgateway.sh stop fix_gwy_3.cfg

