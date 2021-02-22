// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __MarketDataRequestLogger_h__
#define __MarketDataRequestLogger_h__

#include "MarketDataRequestTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class MarketDataRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : MarketDataRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.MDReqID : " << ddsMsg.MDReqID << std::endl
			 << "ddsMsg.SubscriptionRequestType : " << ddsMsg.SubscriptionRequestType << std::endl
			 << "ddsMsg.MarketDepth : " << ddsMsg.MarketDepth << std::endl
			
		;out << "ddsMsg.c_NoMDEntryTypes" << std::endl; 
	 	out << "{" << std::endl;

		for ( int tt = 0; tt < ddsMsg.c_NoMDEntryTypes.length(); tt++)
		{

			 ;out << "ddsMsg.c_NoMDEntryTypes[" << tt << "].MDEntryType : " << ddsMsg.c_NoMDEntryTypes[tt].MDEntryType << std::endl;
		};

		;out << "}" << std::endl;
			
		;out << "ddsMsg.c_NoRelatedSym" << std::endl; 
	 	out << "{" << std::endl;

		for ( int tt = 0; tt < ddsMsg.c_NoRelatedSym.length(); tt++)
		{

			 ;out << "ddsMsg.c_NoRelatedSym[" << tt << "].Symbol : " << ddsMsg.c_NoRelatedSym[tt].Symbol << std::endl;
			 ;out << "ddsMsg.c_NoRelatedSym[" << tt << "].SecurityExchange : " << ddsMsg.c_NoRelatedSym[tt].SecurityExchange << std::endl;
		};

		;out << "}" << std::endl;
;			out << "}";
		out << std::endl;};

};

#endif

