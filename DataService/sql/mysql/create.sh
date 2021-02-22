#!/bin/sh
  
DB_USER=$1
DB_PASS=$2

echo DistributedATS database setup
mysql -u${DB_USER} -p${DB_PASS} --execute="source setup_mysql.sql";
