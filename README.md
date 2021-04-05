# Distributed ATS


DistributedATS is a [**FIX Protocol**](https://www.fixtrading.org) based alternative trading system that integrates [QuickFIX](http://www.quickfixengine.org) and [LiquiBook](https://portal.ociweb.com/products/liquibook) over [OpenDDS](https://www.opendds.org). This project simplifies the process of communication between multiple FIX gateways and multiple matching engines(CLOB) in realtime. FIX Gateways communicate with clients via FIX and with Matching Engines and Middleware(Data Service) components via topic based DDS IDL Pub/Sub mechanism.

## Components
* [**FIX Gateways**](https://github.com/mkipnis/DistributedATS/tree/master/FIXGateway/src) communicate with clients via FIX and with Matching Engines and Data Services via DDS. FIX Gateway converts FIX messages into DDS IDL and publishes them to Matching Engines or Data Services, it converts DDS IDL messages received from Matching Engines and Data Services into FIX and sends them to FIX clients.

* [**Matching Engines**](https://github.com/mkipnis/DistributedATS/tree/master/MatchingEngine/src) maintain orderbooks, publishes market data(conflated), matches orders and publishes IDL based Execution Reports. Matching Engine services all critical to the order flow DDS messages including *NewOrderSingle*, *OrderCancelRequest*, *MassOrderCancel*, etc.

* [**Data Services**](https://github.com/mkipnis/DistributedATS/tree/master/DataService/src) authenticate users, provides reference data, services mass order status requests and market data snapshots. Data Service services all non-critical for orderflow DDS messages including *Logon*, *Logout*, *MassOrderStatusRequest*, *MarketDataSnapshot*.  Data Service can service one or more FIX Gateways and Matching Engines.

![N|Solid](https://github.com/mkipnis/DistributedATS/blob/master/Diagrams/BasicATS.png?raw=true)

[See: BasicATS](README.md#BasicATS)

### Dependences 

|Dependency|Component|
|----------|---------|
|[OpenDDS](https://www.opendds.org)|[DDS/Middleware](https://www.dds-foundation.org/what-is-dds-3/)|
|[QuickFIX](http://www.quickfixengine.org)|[FIXGateway](https://github.com/mkipnis/DistributedATS/tree/master/FIXGateway)|
|[LiquiBook](https://portal.ociweb.com/products/liquibook)|[MatchingEngine](https://github.com/mkipnis/DistributedATS/tree/master/MatchingEngine)|
|[MySQL](https://www.mysql.com)|[DataService](https://github.com/mkipnis/DistributedATS/tree/master/DataService)|

### High Level Data Flow Overview
* FIX gateways convert received from clients FIX messages into DDS IDL structures and publishes them to corresponding DDS topics such as *NEW_ORDER_SINGLE_TOPIC* or *INSTRUMENT_LIST_REQUEST_TOPIC*.  

* Matching Engines and Data Services subscribe to listed above DDS topics, process received IDL messages and publish results to corresponding to DDS topics such as *EXECUTION_REPORT_TOPIC* or *MASS_STATUS_REPORT_TOPIC*.  

* FIX Gateways subscribe to IDL messages published by Matching Engines and Data Services, convert them to FIX format and publish FIX messages back to clients.



#### FIX Gateways, Matching Engines and Data Services utilize DDS content filters to filter in published on DDS messages.  [See: Chapter 5 - Content Subscription Profile](http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf)




Example of Matching Engine filter expression: see [MatchingEngine/src/main.cpp](https://github.com/mkipnis/DistributedATS/blob/master/MatchingEngine/src/main.cpp) 
```
// Filter expression for the MARKET_NAME specified in config file
// filter for messages for this market/security exchange 
std::string filter_str = "m_Header.TargetCompID = 'MATCHING_ENGINE' and SecurityExchange='" + market->getMarketName() + "'";
// filter for mass cancel
std::string filter_str_target_only = "m_Header.TargetCompID = 'MATCHING_ENGINE'"; 

    //  Incoming data
    ///////////////////////////////////////////////////////////////////////////////////
    // New Order Single:
    // Topic
    DDS::Topic_var new_order_single_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_NewOrderSingle::NewOrderSingleTypeSupport_var,
      DistributedATS_NewOrderSingle::NewOrderSingleTypeSupportImpl>(
            NEW_ORDER_SINGLE_TOPIC_NAME);
    // Filter
    DDS::ContentFilteredTopic_ptr cft =
        participant.getDomainParticipant()->create_contentfilteredtopic(
            "FILTER_MATCHING_ENGINE_NEW_ORDER_SINGLE", new_order_single_topic,
            filter_str.c_str(), DDS::StringSeq());
    // Data Reader
    DDS::DataReaderListener_var newOrderSingleDataListener(
        new MatchingEngine::NewOrderSingleDataReaderListenerImpl(market));
    participant.createDataReaderListener(cft, newOrderSingleDataListener);
```

Example of a NewOrderSingle received from FIX Client(TRADER_1) on FIX Gateway(FIX_GATEWAY_1) for Symbol(Tag:55) AAA and SecurityExchange(Tag:207) MARKET_Y.

```
20210222-18:25:23.093908000 : 8=FIX.4.4|9=222|35=8|34=7|49=FIX_GWY_1|52=20210222-18:25:23.093852|56=TRADER_1|6=0.|14=0|17=1000001|31=0|32=0|37=1614018323:MARKET_Y:AAA:7|38=150|39=0|44=1075|54=2|55=AAA|58=OK|
60=20210222-18:25:23.089041|103=0|150=0|151=150|207=MARKET_Y|10=023|
```

Below is a logged version of NewOrderSingle IDL converted by FIX Gateway and published to Matching Engine.  Note, FIX Gateway sets header.TargetSubID to assigned to it Data Service.  This is required for a copy of an Execution Report published by Matching Engine to be consumed by assigned Data Service in order to process consequent order mass status requests.

```
(3599678|3599684|2021-02-22 18:25:23.088947) NewOrderSingle : Message : NewOrderSingle {
Header {
	ddsMsg.BeginString : FIX.4.4
	ddsMsg.BodyLength : 164
	ddsMsg.MsgType : D
	ddsMsg.SenderCompID : FIX_GWY_1
	ddsMsg.TargetCompID : MATCHING_ENGINE
	ddsMsg.MsgSeqNum : 5
	ddsMsg.SenderSubID : TRADER_1
	ddsMsg.TargetSubID : DATA_SERVICE_A
	ddsMsg.SendingTime : 2459268
	}
	ddsMsg.ClOrdID : 1614018323:MARKET_Y:AAA:7
	ddsMsg.Symbol : AAA
	ddsMsg.SecurityExchange : MARKET_Y
	ddsMsg.Side : 2
	ddsMsg.TransactTime : 2459268
	ddsMsg.OrderQty : 150
	ddsMsg.OrdType : 2
	ddsMsg.Price : 1075
	ddsMsg.Text :
}
```
NewOrderSingle IDL above is being consumed and processed by a MatchingEngine with the following filter expression on *NEW_ORDER_SINGLE_TOPIC*: m_Header.TargetCompID = 'MATCHINE_ENGINE' and ddsMsg.SecurityExchange='MARKET_Z'.
Upon receiving and processing of NewOrderSingle above, Matching Engine publishes the following execution report on EXECUTION_REPORT_TOPIC:

```
(3599678|3599684|2021-02-22 18:25:23.089071) ExecutionReport : Message : ExecutionReport {
Header {
	ddsMsg.BeginString :
	ddsMsg.BodyLength : 30979815
	ddsMsg.MsgType : 8
	ddsMsg.SenderCompID : MATCHING_ENGINE
	ddsMsg.TargetCompID : FIX_GWY_1
	ddsMsg.MsgSeqNum : 0
	ddsMsg.SenderSubID : TRADER_1
	ddsMsg.TargetSubID : DATA_SERVICE_A
	ddsMsg.SendingTime : 140647325020862
	}
	ddsMsg.OrderID : 1614018323:MARKET_Y:AAA:7
	ddsMsg.OrigClOrdID :
	ddsMsg.ExecID : 1000001
	ddsMsg.ExecType : 0
	ddsMsg.OrdStatus : 0
	ddsMsg.OrdRejReason : 0
	ddsMsg.Symbol : AAA
	ddsMsg.SecurityExchange : MARKET_Y
	ddsMsg.Side : 2
	ddsMsg.OrderQty : 150
	ddsMsg.Price : 1075
	ddsMsg.LastQty : 0
	ddsMsg.LastPx : 0
	ddsMsg.LeavesQty : 150
	ddsMsg.CumQty : 0
	ddsMsg.AvgPx : 0
	ddsMsg.TransactTime : 1614018323089041
	ddsMsg.Text : OK
}

``` 

FIX Gateway with the following filter expression: header.TargetCompID='FIX_GATEWAY_1' receives IDL Execution Report from *EXECUTION_REPORT_TOPIC*, converts it to FIX and sends FIX Message(8) to FIX client TRADER_1(header.SenderSubID). 

Data Service with the following filter expression: header.TargetSubID='DATA_SERVICE_1' receives a copy of IDL Execution Report published by the Matching Engine on EXECUTION_REPORT_TOPIC and stores it in the internal data structure.  Data Service stores IDL Execution Reports in order to service Mass Status Requests that can be sent by FIX Clients through FIX Gateways on MASS_STATUS_REQUEST_TOPIC.

## BasicATS
[BasicATS](https://github.com/mkipnis/DistributedATS/tree/master/BasicATS) is an example of a trading system with the following setup:	
* Three FIX Gateway : **FIX_GWY_1**, **FIX_GWY_2**, **FIX_GWY_3**
* Two Matching Engines : **MATCHING_ENGINE.MARKET_Y**, **MATCHING_ENGINE.MARKET_Z** 
* Two DataServices : **DATA_SERVICE_A**, **DATA_SERVICE_B**.
	
**FIX_GWY_1** and **FIX_GWY_2** are paired with **DATA_SERVICE_A**, **FIX_GWY_3** is paired with **DATA_SERVICE_B**.  
**MATCHING_ENGINE.MARKET_Y** is paired with **DATA_SERVICE_A** and **MATCHING_ENGINE.MARKET_Z** is paired with **DATA_SERVICE_B**.


Pairing of FIX Gateways and Matching Engines with DataService is done in config files, see configs below.

|Component|Config|
|---------|---------|
|DATA_SERVICE_A|[data_service_a.ini](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/data_service_a.ini)|
|DATA_SERVICE_B|[data_service_b.ini](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/data_service_b.ini)|
|FIX_GWY_1|[fix_gwy_1.cfg](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/fix_gwy_1.cfg)|
|FIX_GWY_2|[fix_gwy_2.cfg](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/fix_gwy_2.cfg)|
|FIX_GWY_3|[fix_gwy_3.cfg](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/fix_gwy_3.cfg)|
|MATCHING_ENGINE.MARKET_Y|[matching_engine_MARKET_Y.ini](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/matching_engine_MARKET_Y.ini)|
|MATCHING_ENGINE.MARKET_Z|[matching_engine_MARKET_Z.ini](https://github.com/mkipnis/DistributedATS/blob/master/BasicATS/config/matching_engine_MARKET_Z.ini)|

BasicATS can be started using start_ats.sh
```
cd BasicATS/scripts
./start_ats.sh
```

Distributed ATS needs to be built and DataService database(MySQL) needs to be setup prior to running BasicATS.
(See: Building Distributed ATS and Setting Up DataService database)

## LatencyTest
[LatencyTest](https://github.com/mkipnis/DistributedATS/tree/master/LatencyTest) is a NewOrderSingle path latency measurement utility. LatencyTest correlates critical to order flow messages across DistributedATS hops by serving as a FIX and DDS IDL client.
It sends NewOrderSingle to FIXGateway, captures converted by FIXGateway and published to Matching Engine NewOrderSingleIDL, captures published by Matching Engine to FIXGateway ExecutionReport IDL, captures published by FIXGateway ExecutionReport, then correlates timestamps, displays latency between hops as well as overall order flow latency.

Note: BasicATS needs to be up and running prior to running LatencyTest
  
## SimulatorATS
[SimulatorATS](https://github.com/mkipnis/DistributedATS/tree/master/SimulatorATS)  continuously simulates the following activity on BasicATS: Consumes market data and aggress on it with random quantities, cancels unfilled orders, sends mass cancel requests.

Note: BasicATS needs to be up and running prior to running SimulatorATS

### Autogeneration of IDL from QuickFIX XML
[GenTools](https://github.com/mkipnis/DistributedATS/tree/master/GenTools) is a utility that generates DDS IDL, FIX to IDL and IDL to FIX adapters, as well as IDL logger helper classes from QuickFIX's XML data dictionary.  Auto generated classes are limited to a set of messages and fields that are specified in the XML config file : [dats_requirements.xml](https://github.com/mkipnis/DistributedATS/blob/master/GenTools/spec/dats_requirements.xml)

Autogenerated files to handle NewOrderSingle:

|Type|File|
|----|----|
|IDL|[NewOrderSingle.idl](https://github.com/mkipnis/DistributedATS/blob/master/GenTools/idl/NewOrderSingle.idl)|
|Adapter/hpp|[NewOrderSingleAdapter.hpp](https://github.com/mkipnis/DistributedATS/blob/master/GenTools/idl/NewOrderSingleAdapter.hpp)|
|Adapter/cpp|[NewOrderSingleAdapter.cpp](https://github.com/mkipnis/DistributedATS/blob/master/GenTools/idl/NewOrderSingleAdapter.cpp)|
|Logger|[NewOrderSingleLogger.hpp](https://github.com/mkipnis/DistributedATS/blob/master/GenTools/idl/NewOrderSingleLogger.hpp)|

Example of listed above classes used in FIXGateway : https://github.com/mkipnis/DistributedATS/blob/master/FIXGateway/src/Application.cpp:
```
void DATSApplication::onMessage(const FIX44::NewOrderSingle &message,
                                const FIX::SessionID &sessionID) {
				
  publishToDDS<NewOrderSingleAdapter,
    DistributedATS_NewOrderSingle::NewOrderSingleDataWriter_ptr,
    DistributedATS_NewOrderSingle::NewOrderSingle, NewOrderSingleLogger>(
      message, _dataWriterContainer->_newOrderSingleDataWriter,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
      
}
```

Publish to DDS template function signature:
```
        template<class ADAPTER, class DATA_WRITER, class DATA, class LOGGER >
        void publishToDDS( const FIX::Message& message, DATA_WRITER& dataWriter,
			 const std::string& sender, const std::string& target,
			 const std::string& senderSubID );
```

## Brief description of some the basic message flows

### Authentication message flow
Upon receiving FIX Logon(A) request from a client, FIX Gateway converts it to an IDL and publishes it on LOGON_TOPIC to a paired with it DataService. DataService consumes IDL Logon request, authenticate it against the database and publishes login back on LOGON_TOPIC with m_Header.SenderCompID='AUTH'.  All FIX gateways subscribe to LOGON_TOPIC with filter expression m_Header.SenderCompID='AUTH' so that any active session for a user which is not the one that is being authenticated is terminated.  This allows user to connect to any gateway while ensuring that only current/latest session is active on the trading system.
However, in order to receive status reports of previously submitted orders, user should only connect to FIX Gateways that are paired with the same DataService.

### Matching Engine Startup
Upon the start up, Matching Engine publishes a security list request on **SECURITY_LIST_REQUEST_TOPIC** to a paired with it DataService.
DataService publishes back security list assigned to a given matching engine on **SECRUTIY_LIST_TOPIC**.  Matching Engine creates order books for all received securities and publishes Market Data Request on **MARKET_DATA_REQUEST_TOPIC** to a paired with it DataService in order to receive opening prices.  DataService replies back with market data snapshot that contains opening prices on **MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC**.

![N|Solid](https://github.com/mkipnis/DistributedATS/blob/master/Diagrams/me_startup.png?raw=true)

### Mass cancel request

Current implementation of MassCancelRequest doesn't require SecuritiesExchange to be specified, so upon the conversion to IDL from FIX, all matching engine will receive masscancelrequest where SenderSubID is being set to client's FIX SenderCompID.  Note: MassCancelRequest is also getting sent from FIX gateway on an event when client logs out or disconnects.

## FIX Messages, Topics, Sources and Destinations

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

## Building Distributed ATS

All DistributedATS dependences need to be built and MySQL needs to be installed prior to building the project.

[See: Dependences](README.md#Dependences)

The following environmental variables along with DDS variables must be set prior to building the project: DATS_HOME(DistributedATS home), QUICKFIX_HOME, MYSQL_HOME, LIQUIDBOOK_HOME as well as LD_LIBRARY_PATH. 

Example of an environmental variable file where all dependences were built under $HOME and MySQL was installed in its default location:
```
. $HOME/OpenDDS-3.9/setenv.sh # OpenDDS and it's dependences

export DATS_HOME=$HOME/DistributedATS # DistributedATS home

export QUICKFIX_HOME=$HOME/quickfix
export MYSQL_HOME=/usr/local/mysql
export LIQUIDBOOK_HOME=$HOME/liquibook-master
export DYLD_LIBRARY_PATH=$HOME/quickfix/src/C++/.libs:$DYLD_LIBRARY_PATH
```

DistributedATS utilizes Make Project Creator as a build tool (Make Project Creator)[http://downloads.ociweb.com/MPC/docs/html/MakeProjectCreator.html]

Upon successfully building and installing all the dependences, the following set of commands can be used to build the project:
```
$ACE_ROOT/bin/mwc.pl -type gnuace
make realclean
make
```

## Setting Up DataService database

Currently, DataServices supports MySQL only.  MySQL needs to be installed and root user setup prior to setting up database for DataService.
Upon the installation of MySQL and setup of its root user, run create.sh script to setup DistributedATS database.

```
cd DataService/sql/mysql
./create.sh root password
```
The following schema with related stored procedures and sample data for BasicATS will be created after running create.sh
![N|Solid](https://github.com/mkipnis/DistributedATS/blob/master/Diagrams/distributed_ats_db.png?raw=true)

## Front-End

See [WebTrader](https://github.com/mkipnis/WebTrader) - an interactive web based front-end.

## US Treasuries - Central Limit Order Book (CLOB)

### Loading reference data

```
cd 
cd $DATS_HOME/StaticData/UST/USTDataSaver/
mvn install
cd scripts
./load_data.sh
```
```load_data.sh``` loads static data from $DATS_HOME/StaticData/UST/Data/*.json into the database, see ```select ref_data from instrument_ref_data``` for populated fields in JSON format

This script also setups matching engine for market name 'UST' and maps users in TRADER_GROUP_A and TRADER_GROUP_B to 'UST' market.

### Start BasicATS then start matching engine for 'UST' market 
```
cd $DATS_HOME/MatchingEngine/scripts
./matchingengine.sh start matching_engine_UST.ini 
```
### Other CLOBs: inverted order book, zero and negative prices/rates/spreads
The following version of LiquiBook is required in order to support inverted book, zero and negative prices/rates/spreads:
![N|Solid][LiquiBook(Inverted Book, Zero and Negative Price/Rate/Spread)](https://github.com/mkipnis/liquibook)

Sample Repo instrument setup:
```
$DATS_HOME/StaticData/Repo/Data/sample_repo.sql
```
