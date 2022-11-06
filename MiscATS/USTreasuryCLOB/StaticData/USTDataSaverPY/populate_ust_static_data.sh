#!/bin/sh

python3 USTDataSaverPY.py -s ../Data/ust_bills_20210301.json -d ../../../../DataService/sql/sqlite/distributed_ats.db
python3 USTDataSaverPY.py -s ../Data/ust_notes_20210301.json -d ../../../../DataService/sql/sqlite/distributed_ats.db
python3 USTDataSaverPY.py -s ../Data/ust_bonds_20210301.json -d ../../../../DataService/sql/sqlite/distributed_ats.db
