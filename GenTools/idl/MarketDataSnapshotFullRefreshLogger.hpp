// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __MarketDataSnapshotFullRefreshLogger_h__
#define __MarketDataSnapshotFullRefreshLogger_h__

#include "MarketDataSnapshotFullRefreshTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class MarketDataSnapshotFullRefreshLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : MarketDataSnapshotFullRefresh { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.MDReqID : " << ddsMsg.MDReqID << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
			
		;out << "ddsMsg.c_NoMDEntries" << std::endl; 
	 	out << "{" << std::endl;

		for ( int tt = 0; tt < ddsMsg.c_NoMDEntries.length(); tt++)
		{

			 ;out << "ddsMsg.c_NoMDEntries[" << tt << "].MDEntryType : " << ddsMsg.c_NoMDEntries[tt].MDEntryType << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries[" << tt << "].MDEntryPx : " << ddsMsg.c_NoMDEntries[tt].MDEntryPx << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries[" << tt << "].MDEntrySize : " << ddsMsg.c_NoMDEntries[tt].MDEntrySize << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries[" << tt << "].Text : " << ddsMsg.c_NoMDEntries[tt].Text << std::endl;
		};

		;out << "}" << std::endl;
;			out << "}";
		out << std::endl;};

};

#endif

