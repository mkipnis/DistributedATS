// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderCancelRequestLogger_h__
#define __OrderCancelRequestLogger_h__

#include "OrderCancelRequestTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class OrderCancelRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderCancelRequest::OrderCancelRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderCancelRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.OrigClOrdID : " << ddsMsg.OrigClOrdID << std::endl
			 << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
			 << "ddsMsg.Side : " << ddsMsg.Side << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime << std::endl
			 << "ddsMsg.OrderQty : " << ddsMsg.OrderQty << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

