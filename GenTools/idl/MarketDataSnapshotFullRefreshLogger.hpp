/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class MarketDataSnapshotFullRefreshLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : MarketDataSnapshotFullRefresh { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
			 << "ddsMsg.MDReqID : " << ddsMsg.MDReqID() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
			
		;out << "ddsMsg.c_NoMDEntries" << std::endl; 
	 	out << "{" << std::endl;

		for ( int tt = 0; tt < ddsMsg.c_NoMDEntries().size(); tt++)
		{

			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDEntryType : " << ddsMsg.c_NoMDEntries()[tt].MDEntryType() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDEntryPx : " << ddsMsg.c_NoMDEntries()[tt].MDEntryPx() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDEntrySize : " << ddsMsg.c_NoMDEntries()[tt].MDEntrySize() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].TimeInForce : " << ddsMsg.c_NoMDEntries()[tt].TimeInForce() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].ExecInst : " << ddsMsg.c_NoMDEntries()[tt].ExecInst() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].Text : " << ddsMsg.c_NoMDEntries()[tt].Text() << std::endl;
		};

		;out << "}" << std::endl;
;			out << "}";
		out << std::endl;};

};
