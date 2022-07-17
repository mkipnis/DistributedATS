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

