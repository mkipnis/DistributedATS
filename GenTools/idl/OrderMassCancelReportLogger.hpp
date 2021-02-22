// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderMassCancelReportLogger_h__
#define __OrderMassCancelReportLogger_h__

#include "OrderMassCancelReportTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class OrderMassCancelReportLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassCancelReport { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.OrderID : " << ddsMsg.OrderID << std::endl
			 << "ddsMsg.MassCancelRequestType : " << ddsMsg.MassCancelRequestType << std::endl
			 << "ddsMsg.MassCancelResponse : " << ddsMsg.MassCancelResponse << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

