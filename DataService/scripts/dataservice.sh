#!/bin/bash

#set -x

PRG="$0"
PROGNAME=DataService
CONFIG_FILE_NAME=$2

export LOG_FILE_NAME=$2.log

. $DATS_HOME/dats_env.sh

logfile=$BASEDIR_ATS/logs/$PROGNAME.$CONFIG_FILE_NAME.console.log

#ldd $DATS_HOME/bin/$PROGNAME >> $logfile

PROCESS="$PROGNAME -c $BASEDIR_ATS/config/$CONFIG_FILE_NAME"
#echo PROCESS=$PROCESS >> $logfile

idn=$(id -u)

case $1 in
        start)
        	echo "Starting : "$PROCESS 
		$0 check $CONFIG_FILE_NAME
		rc=$?;

		if [ "$rc" == "0" ]
		then
			$DATS_HOME/bin/$PROCESS >> $logfile 2>&1 &
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
#		echo "Checking :" $PROCESS_CHECK
		DATASERVICE_PID=`pgrep -f "$PROCESS_CHECK"`
		# echo $DATASERVICE_PID >> $logfile;
		if [ ! -z "$DATASERVICE_PID" ]
		then
        		echo "DataService [$CONFIG_FILE_NAME] is running - $DATASERVICE_PID";
        	#	echo "DataService [$CONFIG_FILE_NAME] is running - $DATASERVICE_PID" >> $logfile;
        		exit 1;
		else
        		echo "DataService [$CONFIG_FILE_NAME] is not running $logfile";
        	#	echo "DataService [$CONFIG_FILE_NAME] is not running" >> $logfile;
			exit 0;
		fi
        ;;
        *)
        echo "unknown command"
        exit -1;
        ;;
esac

echo $(date '+%Y%m%d.%H%M%S') run-done : pid,$DATASERVICE_PID >> $logfile
