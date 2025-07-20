## Matching Engine

USTreasuryCLOB ATS consists of a single Matching Engine, two FIX Gateways, and two Data Services.
The matching engine consumes and processes orders from the instrument group MARKET_UST that covers 300+ instruments

![N|Solid](https://raw.githubusercontent.com/mkipnis/DistributedATS/master/docs/Diagrams/USTreasuryCLOB.png?raw=true)


### Components

| Type | Name     | Config                |
| :-------- | :------- | :------------------------- |
| Matching Engine | MARKET_UST | [matching_engine_MARKET_UST.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/USTreasuryCLOB/config/matching_engine_MARKET_UST.ini) |
| Data Service | DATA_SERVICE_A | [data_service_a.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/USTreasuryCLOB/config/data_service_a.ini) |
| Data Service | DATA_SERVICE_B | [data_service_b.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/USTreasuryCLOB/config/data_service_b.ini) |
| FIX Gateway | FIX_GWY_1 | [fix_gwy_1.cfg](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/USTreasuryCLOB/config/fix_gwy_1.cfg)|
| FIX Gateway | FIX_GWY_2 | [fix_gwy_2.cfg](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/USTreasuryCLOB/config/fix_gwy_2.cfg) |


### Setup US Treasury Instruments
```
    $DATS_HOME/MiscATS/USTreasuryCLOB/StaticData/USTDataSaverPY
```

### Starting ATS

```
    cd $DATS_HOME/MiscATS/USTreasuryCLOB/scipts
    ./start_ats
```

## Test client
- [ReactJS and SpringBoot ](https://github.com/mkipnis/DistributedATS/tree/master/misc_clients/spring_reactjs)
- Username/Password: UST_TRADER_1/TEST
