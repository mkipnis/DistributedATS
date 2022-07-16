## Brief description of some the basic message flows

### Authentication message flow
Upon receiving FIX Logon(A) request from a client, FIX Gateway converts it to an IDL and publishes it on LOGON_TOPIC to a paired with it DataService. DataService consumes IDL Logon request, authenticate it against the database and publishes login back on LOGON_TOPIC with m_Header.SenderCompID='AUTH'.  All FIX gateways subscribe to LOGON_TOPIC with filter expression m_Header.SenderCompID='AUTH' so that any active session for a user which is not the one that is being authenticated is terminated.  This allows user to connect to any gateway while ensuring that only current/latest session is active on the trading system.
However, in order to receive status reports of previously submitted orders, user should only connect to FIX Gateways that are paired with the same DataService.

### Matching Engine Startup
Upon the start up, Matching Engine publishes a security list request on **SECURITY_LIST_REQUEST_TOPIC** to a paired with it DataService.
DataService publishes back security list assigned to a given matching engine on **SECRUTIY_LIST_TOPIC**.  Matching Engine creates order books for all received securities and publishes Market Data Request on **MARKET_DATA_REQUEST_TOPIC** to a paired with it DataService in order to receive opening prices.  DataService replies back with market data snapshot that contains opening prices on **MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC**.

![N|Solid](https://github.com/mkipnis/DistributedATS/blob/master/docs/Diagrams/me_startup.png?raw=true)

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
