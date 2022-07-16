## Crypto Central Limit Order Book

CryptoCLOB ATS consists of three Matching Engines, three FIX Gateways, and two Data Services.
The first matching engine consumes and processes orders from the instrument group BTC_MARKET, the second from the group ETH_MARKET, and the third from OTHER_COIN_MARKET

![N|Solid](https://raw.githubusercontent.com/mkipnis/DistributedATS/master/docs/Diagrams/CryptoCLOB.png?raw=true)


### Components

| Type | Name     | Config                |
| :-------- | :------- | :------------------------- |
| Matching Engine | BTC_MARKET | [matching_engine_MARKET_BTC.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/matching_engine_MARKET_BTC.ini) |
| Matching Engine | ETH_MARKET | [matching_engine_MARKET_ETH.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/matching_engine_MARKET_ETH.ini) |
| Matching Engine | OTHER_COIN_MARKET | [matching_engine_MARKET_OTHER_COIN.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/matching_engine_MARKET_OTHER_COIN.ini) |
| Data Service | DATA_SERVICE_A | [data_service_a.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/data_service_a.ini) |
| Data Service | DATA_SERVICE_B | [data_service_b.ini](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/data_service_b.ini)  |
| FIX Gateway | FIX_GWY_1 | [fix_gwy_1.cfg](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/fix_gwy_1.cfg) |
| FIX Gateway | FIX_GWY_2 | [fix_gwy_2.cfg](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/fix_gwy_2.cfg) |
| FIX Gateway | FIX_GWY_3 | [fix_gwy_3.cfg](https://github.com/mkipnis/DistributedATS/blob/master/MiscATS/CryptoCLOB/config/fix_gwy_3.cfg) |


### Prerequisites
- [Building Distributed ATS](https://github.com/mkipnis/DistributedATS#Building-Distributed-ATS)
- [DataService Setup](https://github.com/mkipnis/DistributedATS/tree/master/DataService/sql/mysql)

### Loading Data
```
    cd $DATS_HOME/MiscATS/CryptoCLOB/sql
    $ mysql -u dataservice distributed_ats -p

    mysql> source load_sample_data.sql;
```

### Starting ATS

```
    cd $DATS_HOME/MiscATS/CryptoCLOB/scipts
    ./start_ats
```

## Test client
- [ReactJS and SpringBoot ](https://github.com/mkipnis/DistributedATS/tree/master/MiscClients/spring_reactjs)
- Username/Password: CRYPTO_TRADER_1/TEST
