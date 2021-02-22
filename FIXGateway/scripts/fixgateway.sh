#!/bin/bash

set -x

PRG="$0"
PROGNAME=FIXGateway
CONFIG_FILE_NAME=$2

if [ "$1" == "start" ] || [ "$1" == "stop" ] || [ "$1" == "check" ] ;
then
	echo "Supported Command - "$1;
else
        echo "Unsupported Command - "$1 : "Supported Commands : (start/stop/check)";
	exit 1;
fi

if [ -z "$CONFIG_FILE_NAME" ]
then
        echo "Config File Name is not set";
        exit 1;
fi

#PRGDIR=`dirname "$PRG"`

if [ -z "$BASEDIR" ]
then
	BASEDIR=`cd ".." > /dev/null; pwd`
fi

if [ ! -d "$BASEDIR/logs/" ]
then
        mkdir $BASEDIR/logs/
fi

logfile=$BASEDIR/logs/$PROGNAME.$CONFIG_FILE_NAME.startstop.log

#ldd $DATS_HOME/bin/$PROGNAME >> $logfile

PROCESS="$PROGNAME -c $BASEDIR/config/$CONFIG_FILE_NAME -DCPSConfigFile $DATS_HOME/Protocols/tcp.ini -ORBLogFile $BASEDIR/logs/$PROGNAME.$CONFIG_FILE_NAME.log"
echo PROCESS=$PROCESS >> $logfile

idn=$(id -u)

case $1 in
        start)
        	echo "Starting : "$PROCESS 
		$0 check $CONFIG_FILE_NAME
		rc=$?;

		if [ "$rc" == "0" ]
		then
			nohup $DATS_HOME/bin/$PROCESS & >> $logfile 2>&1
			sleep 1
			$0 check $CONFIG_FILE_NAME
       			exit 1;	
		fi

		$0 check $CONFIG_FILE_NAME
        ;;
        stop)
        echo "Stoping : "$PROCESS

		pkill -SIGTERM -f "$PROCESS"
		sleep 1

		$0 check $CONFIG_FILE_NAME

		rc=$?;
		
		if [ "$rc" == "1" ]
		then
			cnt=0
			while [ true ]
			do
        			pid=$(pgrep  -U $idn -f  "$PROCESS")
        			if [ "$pid" == "" ]
        			then
                			break
        			fi
        			sleep 1
        			if [ $cnt -gt 10 ]
        			then
                			break
        			fi
        			cnt=$(($cnt+1))
			done

			pkill -KILL -U $idn -f "$PROCESS"
		fi
	
        ;;
        check)
		#PROCESS_CHECK="$PROGNAME.*$CONFIG_FILE_NAME"
		PROCESS_CHECK="$PROCESS"
        	echo "Checking :" $PROCESS_CHECK
		FIXGATEWAY_PID=`pgrep -f "$PROCESS_CHECK"`
		echo $FIXGATEWAY_PID >> $logfile;
		if [ ! -z "$FIXGATEWAY_PID" ]
		then
        		echo "FIXGateway [$CONFIG_FILE_NAME] is running - $FIXGATEWAY_PID";
        		echo "FIXGateway [$CONFIG_FILE_NAME] is running - $FIXGATEWAY_PID" >> $logfile;
        		exit 1;
		else
        		echo "FIXGateway [$CONFIG_FILE_NAME] is not running $logfile";
        		echo "FIXGateway [$CONFIG_FILE_NAME] is not running" >> $logfile;
			exit 0;
		fi
        ;;
        *)
        echo "unknown command"
        exit -1;
        ;;
esac

echo $(date '+%Y%m%d.%H%M%S') run-done : pid,$FIXGATEWAY_PID >> $logfile
