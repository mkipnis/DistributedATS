#!/bin/bash

# bills
mvn -f ../pom.xml exec:java -D"exec.mainClass"="org.DistributedATS.USTDataSaver" -Dexec.args="../../Data/ust_bills_20210301.json" -Dexec.cleanupDaemonThreads=false

# notes
mvn -f ../pom.xml exec:java -D"exec.mainClass"="org.DistributedATS.USTDataSaver" -Dexec.args="../../Data/ust_notes_20210301.json" -Dexec.cleanupDaemonThreads=false

#bonds
mvn -f ../pom.xml exec:java -D"exec.mainClass"="org.DistributedATS.USTDataSaver" -Dexec.args="../../Data/ust_bonds_20210301.json" -Dexec.cleanupDaemonThreads=false
