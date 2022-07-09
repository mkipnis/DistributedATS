## Multiple Matching Engines

CryptoCLOB ATS consists of three Matching Engines, three FIX Gateways, and two Data Services.
The first matching engine consumes and processes orders from the instrument group BTC_MARKET, the second from the instrument ETH_MARKET, and the third from OTHER_COIN_MARKET

![N|Solid](https://raw.githubusercontent.com/mkipnis/DistributedATS/master/Diagrams/BasicATS.png?raw=true)


### Components

| Type | Name     | Config                |
| :-------- | :------- | :------------------------- |
| Matching Engine | BTC_MARKET | **TODO** |
| Matching Engine | ETH_MARKET | **TODO** |
| Matching Engine | OTHER_COIN_MARKET | **TODO** |
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
- [ReactJS and SpringBoot ](https://github.com/mkipnis/DistributedATS/tree/master/misc_clients/spring_reactjs)
- [Username/Password]: [TRADER_1/TEST].  See **sample_users_data.sql** for details
