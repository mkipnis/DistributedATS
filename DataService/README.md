
## DataService
DataService authenticates users, provides reference data, services mass order status requests, and market data snapshots. Data Service services all non-critical to the order flow DDS messages including *Logon*, *Logout*, *MassOrderStatusRequest*, *MarketDataSnapshot*.  Data Service can service one or more FIX Gateways and Matching Engines.


## Setting up the DataService database

Currently, DataServices supports MySQL only.  MySQL needs to be installed and root user setup prior to setting up the database for DataService.
Upon the installation of MySQL and setup of its root user, run create.sh script to setup DistributedATS database.

```
cd DataService/sql/mysql
./create.sh root password
```
The following schema with related stored procedures and sample data for BasicATS will be created after running create.sh
![N|Solid](https://github.com/mkipnis/DistributedATS/blob/master/docs/Diagrams/distributed_ats_db.png?raw=true)
