## Distributed ATS


DistributedATS is a [**FIX Protocol-based**](https://www.fixtrading.org) alternative trading system that integrates [QuickFIX](http://www.quickfixengine.org) and [LiquiBook](https://portal.ociweb.com/products/liquibook) over [OpenDDS](https://www.opendds.org). This project simplifies the process of communication between multiple FIX gateways and multiple matching engines(CLOB) in real-time. FIX Gateways communicate with clients via FIX and Matching Engines and Middleware(Data Service) components via topic-based DDS IDL Pub/Sub mechanism.

### Components

* [**FIX Gateway**](https://github.com/mkipnis/DistributedATS/tree/master/FIXGateway/src) communicates with clients via FIX and with Matching Engines and Data Services via DDS. FIX Gateway converts FIX messages into DDS IDL and publishes converted IDLs to Matching Engines or Data Services, it converts DDS IDL messages received from Matching Engines and Data Services into FIX and sends them to FIX clients.

* [**Matching Engine**](https://github.com/mkipnis/DistributedATS/tree/master/MatchingEngine/src) maintains order-books, publishes market data(conflated), matches orders, and publishes IDL-based Execution Reports. Matching Engine services critical to the order flow DDS messages including *NewOrderSingle*, *OrderCancelRequest*, *MassOrderCancel*, etc.

* [**Data Service**](https://github.com/mkipnis/DistributedATS/tree/master/DataService/src) authenticates users, provides reference data, services mass order status requests, and market data snapshots. Data Service services all non-critical to order flow DDS messages including *Logon*, *Logout*, *MassOrderStatusRequest*, *MarketDataSnapshot*.  Data Service can service one or more FIX Gateways and Matching Engines.

![N|Solid](https://raw.githubusercontent.com/mkipnis/DistributedATS/master/docs/Diagrams/CryptoCLOB.png?raw=true)
[See: CryptoCLOB](https://github.com/mkipnis/DistributedATS/tree/master/MiscATS/CryptoCLOB)

### Dependences

|Dependency|Component|
|----------|---------|
|[OpenDDS](https://www.opendds.org)|[DDS/Middleware](https://www.dds-foundation.org/what-is-dds-3/)|
|[QuickFIX](http://www.quickfixengine.org)|[FIXGateway](https://github.com/mkipnis/DistributedATS/tree/master/FIXGateway)|
|[LiquiBook](https://portal.ociweb.com/products/liquibook)|[MatchingEngine](https://github.com/mkipnis/DistributedATS/tree/master/MatchingEngine)|
|[MySQL](https://www.mysql.com)|[DataService](https://github.com/mkipnis/DistributedATS/tree/master/DataService)|


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

## Highlevel 

### Autogeneration of IDL from QuickFIX XML
[GenTools](https://github.com/mkipnis/DistributedATS/tree/master/GenTools) is a utility that generates DDS IDL, FIX to IDL, and IDL to FIX adapters and IDL logger helper classes from QuickFIX's XML data dictionary.

## Building Distributed ATS
[Building ATS](https://github.com/mkipnis/DistributedATS/tree/master/docs/BuildingDistributedATS.md)
