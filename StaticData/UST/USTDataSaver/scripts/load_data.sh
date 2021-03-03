#!/bin/bash

mysql --user=dataservice --password=Welcome#1 --database=distributed_ats <<EOFSQL

call insert_market('UST', 'CLOB');
call map_user_group_to_market('TRADER_GROUP_A', 'UST');
call map_user_group_to_market('TRADER_GROUP_B', 'UST');

-- creating users matching engine -- DataService will replay to them with instrument lists
call insert_user('MATCHING_ENGINE', 'MATCHING_ENGINE_UST', 'UST', 'Matching Market UST');

call map_user_group_to_market('MATCHING_ENGINE_UST', 'UST');

EOFSQL

# bills
mvn -f ../pom.xml exec:java -D"exec.mainClass"="org.DistributedATS.USTDataSaver" -Dexec.args="${DATS_HOME}/StaticData/UST/Data/ust_bills_20210301.json" -Dexec.cleanupDaemonThreads=false

# notes
mvn -f ../pom.xml exec:java -D"exec.mainClass"="org.DistributedATS.USTDataSaver" -Dexec.args="${DATS_HOME}/StaticData/UST/Data/ust_notes_20210301.json" -Dexec.cleanupDaemonThreads=false

#bonds
mvn -f ../pom.xml exec:java -D"exec.mainClass"="org.DistributedATS.USTDataSaver" -Dexec.args="${DATS_HOME}/StaticData/UST/Data/ust_bonds_20210301.json" -Dexec.cleanupDaemonThreads=false
