
## Multiple Matching Engines

USTreasuryCLOB ATS consists of a single Matching Engine, three FIX Gateways, and two Data Services.
The matching engine consumes and processes orders from the instrument group MARKET_UST that covers 300+ instrument

![N|Solid](https://raw.githubusercontent.com/mkipnis/DistributedATS/master/Diagrams/BasicATS.png?raw=true)


### Components

| Type | Name     | Config                |
| :-------- | :------- | :------------------------- |
| Matching Engine | MARKET_UST | **TODO** |
| Data Service | DATA_SERVICE_A | **TODO** |
| Data Service | DATA_SERVICE_B | **TODO** |
| FIX Gateway | FIX_GWY_1 | **TODO** |
| FIX Gateway | FIX_GWY_2 | **TODO** |
| FIX Gateway | FIX_GWY_3 | **TODO** |


### Prerequisites
- [Building Distributed ATS](https://github.com/mkipnis/DistributedATS#Building-Distributed-ATS)
- [DataService Setup](https://github.com/mkipnis/DistributedATS/tree/master/DataService/sql/mysql)

### Loading Data
```
    cd $DATS_HOME/MiscATS/USTreasuryCLOB/sql
    $ mysql -u dataservice distributed_ats -p

    mysql> source load_sample_data.sql;    
```

### Setup US Treasury Instruments
```
    $DATS_HOME/MiscATS/USTreasuryCLOB/StaticData/USTDataSaver
```

### Starting ATS

```
    cd $DATS_HOME/MiscATS/USTreasuryCLOB/scipts
    ./start_ats
```

## Test client
- [ReactJS and SpringBoot ](https://github.com/mkipnis/DistributedATS/tree/master/misc_clients/spring_reactjs)
- [Username/Password]: [TRADER_1/TEST].  See **sample_users_data.sql** for details
