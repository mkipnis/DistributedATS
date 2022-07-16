## LatencyTest
LatencyTest correlates all critical to the order flow messages across all DistributedATS hops(FIXGateway, MatchingEngine). This utility serves as a FIX and DDS IDL client.
LatencyTest sends NewOrderSingle to the FIXGateway, captures converted by the FIXGateway and published to the Matching Engine NewOrderSingleIDLs, captures published by the Matching Engine to the FIXGateway ExecutionReportIDLs,  and captures published by the FIXGateway ExecutionReport.  LatencyTest correlates timestamps and displays microsecond latency between hops and the overall order flow latency. 

Run latency test
-----------------
./LatencyTest  -c LatencyTest.cfg  -DCPSConfigFile $DATS_HOME/Protocols/tcp.ini
