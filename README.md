## Distributed ATS


DistributedATS is a [**FIX Protocol-based**](https://www.fixtrading.org) alternative trading system that integrates [QuickFIX](https://github.com/quickfix/quickfix) and [LiquiBook](https://github.com/enewhuis/liquibook) over [OpenDDS](https://github.com/objectcomputing/OpenDDS). This project simplifies the process of communication between multiple FIX gateways and multiple matching engines(CLOB) in real-time. FIX Gateways communicate with clients via FIX and Matching Engines and Middleware(Data Service) components via topic-based DDS IDL Pub/Sub mechanism.

### Components

* [**FIX Gateway**](https://github.com/mkipnis/DistributedATS/tree/master/FIXGateway/src) communicates with clients via FIX and with Matching Engines and Data Services via DDS. FIX Gateway converts FIX messages into DDS IDL and publishes converted IDLs to Matching Engines or Data Services, it converts DDS IDL messages received from Matching Engines and Data Services into FIX and sends them to FIX clients.

* [**Matching Engine**](https://github.com/mkipnis/DistributedATS/tree/master/MatchingEngine/src) maintains order-books, publishes market data(conflated), matches orders, and publishes IDL-based Execution Reports. Matching Engine services critical to the order flow DDS messages including *NewOrderSingle*, *OrderCancelRequest*, *MassOrderCancel*, etc.

* [**Data Service**](https://github.com/mkipnis/DistributedATS/tree/master/DataService/src) authenticates users, provides reference data, services mass order status requests, and market data snapshots. Data Service services all non-critical to order flow DDS messages including *Logon*, *Logout*, *MassOrderStatusRequest*, *MarketDataSnapshot*.  Data Service can service one or more FIX Gateways and Matching Engines.

![N|Solid](https://raw.githubusercontent.com/mkipnis/DistributedATS/master/docs/Diagrams/CryptoCLOB.png?raw=true)
[See: CryptoCLOB](https://github.com/mkipnis/DistributedATS/tree/master/MiscATS/CryptoCLOB)

### Dependencies

|Dependency|Component|
|----------|---------|
|[OpenDDS](https://www.opendds.org)|[DDS/Middleware](https://www.dds-foundation.org/what-is-dds-3/)|
|[QuickFIX](https://github.com/quickfix/quickfix)|[FIXGateway](https://github.com/mkipnis/DistributedATS/tree/master/FIXGateway)|
|[LiquiBook](https://github.com/enewhuis/liquibook)|[MatchingEngine](https://github.com/mkipnis/DistributedATS/tree/master/MatchingEngine)|
|[SQLite3](https://github.com/sqlite/sqlite)|[DataService](https://github.com/mkipnis/DistributedATS/tree/master/DataService)|


### Sample Exchanges
[MultiMatchingEngineATS](https://github.com/mkipnis/DistributedATS/tree/master/MiscATS/MultiMatchingEngineATS) - Two Matching Engines, three FIX Gateways, and two Data Services. The first matching engine consumes and processes orders from the instrument group MARKET_Y, the second from the instrument MARKET_Z

[USTreasuryCLOB](https://github.com/mkipnis/DistributedATS/tree/master/MiscATS/USTreasuryCLOB) - A single Matching Engine, three FIX Gateways, and two Data Services. The matching engine consumes and processes orders from the instrument group MARKET_UST that covers 300+ instruments (OrderBooks)

[CryptoCLOB](https://github.com/mkipnis/DistributedATS/tree/master/MiscATS/CryptoCLOB) - Three Matching Engines, three FIX Gateways, and two Data Services. The first matching engine consumes and processes orders from the instrument group BTC_MARKET, the second from the instrument ETH_MARKET, and the third from OTHER_COIN_MARKET

### Other Utilities
- [LatencyTest](https://github.com/mkipnis/DistributedATS/tree/master/LatencyTest) - A NewOrderSingle path latency measurement utility. LatencyTest correlates critical to the order flow messages across DistributedATS hops by serving as a FIX and DDS IDL client.
- [SimulatorATS](https://github.com/mkipnis/DistributedATS/tree/master/SimulatorATS) - A random activity simulator that consumes market data, aggress on it with arbitrary quantities, cancels unfilled orders and sends mass cancel requests.

### Sample Clients
- [Python3/QuickFIX](https://github.com/mkipnis/DistributedATS/tree/master/MiscClients/python3)
- [ReactJS and SpringBoot-QuickFIX/J](https://github.com/mkipnis/DistributedATS/tree/master/MiscClients/spring_reactjs)

## High Level Data Flow Overview
[Overview](https://github.com/mkipnis/DistributedATS/tree/master/docs/HighLevelDesign.md)

### FIX Messages, Topics, Sources and Destinations

|FIX Message|DDS Topic|Source|Destination|Content Description|
|-----------|---|------|--------------------------|-----------|
|Login(A)|LOGON_TOPIC|FIXGateway|DataService|Converted to IDL FIX Login|
|Login(A)|LOGON_TOPIC|DataService|FIXGateway|Successfully Authenticated FIX Session by a Data Service |
|Logout(5)|LOGOUT_TOPIC|DataService|FIXGateway|Unsuccessfully Authenticate FIX Session by Data Service|
|NewOrderSingle(D)|NEW_ORDER_SINGLE_TOPIC|FIXGateway|MatchingEngine|New order single|
|ExecutionReport(8)|EXECUTION_REPORT_TOPIC|MatchingEngine|FIXGateway|Execution Report to be sent to a client|
|ExecutionReport(8)|EXECUTION_REPORT_TOPIC|MatchingEngine|DataService|Execution Report to be store to service Order Mass Status Requests|
|OrderCancel Request(F)|ORDER_CANCEL_  REQUEST_TOPIC|FIXGateway|MatchingEngine|Converted to IDL Cancel Request from FIX Client|
|OrderCancel Reject(9)|ORDER_CANCEL_REJECT_ TOPIC|MatchingEngine|FIXGateway|IDL Cancel Reject, if order can't be cancelled|
|OrderCancelReplace Request(G)|ORDER_CANCEL_ REPLACE_REQUEST_TOPIC_NAME|FIXGateway|MatchingEngine|Converted to IDL Cancel Replace Request from FIX Client|
|MassCancel Request(q)|ORDER_MASS_CANCEL_ REQUEST_TOPIC|FIXGateway|MatchingEngines|Mass Cancel requested by FIX Client or FIX Client Logout/Disconnect|
|MassCancel Report(r)|ORDER_MASS_CANCEL_ REPORT_TOPIC|MatchingEngines|FIXGateway|Results of Mass Cancel Request|
|MarketData Request(V)|MARKET_DATA_REQUEST _TOPIC|FIXGateway|DataService|Request for the current state of Order Book(Top 5 levels)|
|MarketDataSnapshotFull Refresh(W)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|DataService|FIXGateway|Current Market Data Snapshot(Top 5 levels)|
|MarketDataSnapshotFull Refresh(W)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|Data Service|MatchingEngine|OpeningPrices|
|MarketDataIncremental Refresh(X)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|MatchingEngine|FIXGateway|Incremental Market Data Request(Top 5 levels)|
|MarketDataIncremental Refresh(X)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|MatchingEngine|FIXGateway|Incremental Market Data Request(Top 5 levels)|
|Security ListRequest(x)|SECURITY_LIST_REQUEST _TOPIC|FIXGateway|DataService|FIX Client initiated Security List Request|
|SecurityList(y)|SECURITY_LIST_TOPIC|DataService|FIXGateway|Security list reply for FIX Client Request|
|SecuritList Request(x)|SECURITY_LIST_REQUEST _TOPIC|MatchingEngine|DataService|Security List Request for Order Book setup|
|SecurityList(y)|SECURITY_LIST_TOPIC|DataService|MatchingEngine|Security List for OrderBook Setup|
|OrderMassStatus Request(AF)|ORDER_MASS_STATUS_ REQUEST_TOPIC|FIXGateway|DataService|Initiated by FIX Client Mass Status Request|

### Autogeneration of IDL from QuickFIX XML
[GenTools](https://github.com/mkipnis/DistributedATS/tree/master/GenTools) is a utility that generates DDS IDL, FIX to IDL, and IDL to FIX adapters and IDL logger helper classes from QuickFIX's XML data dictionary.

### Building Distributed ATS and it's dependencies
```
download_deps_and_build_all.sh
```

### Docker

[Docker YML](https://github.com/mkipnis/DistributedATS/blob/master/docker/docker-compose.yml)

version: '2'
services:
  distributed_ats:
    image: ghcr.io/mkipnis/distributed_ats:latest
    command: ["bash", "-c", "cd /opt/DistributedATS/; . ./distributed_ats_env.sh; cd MiscATS/CryptoCLOB/scripts; ./start_ats.sh"]
    ports:
      - "15001:15001"
      - "16001:16001"
      - "17001:17001"

  distributed_ats_webtrader:
    image: ghcr.io/mkipnis/distributed_ats_webtrader:latest
    ports:
      - "8080:8080"


Upon starting the dockerized instance, open your browser and go to http://localhost:8080. Then, use either CRYPTO_TRADER_1, CRYPTO_TRADER_2, CRYPTO_TRADER_3, or CRYPTO_TRADER_4 as the username to log in with the password "TEST".
