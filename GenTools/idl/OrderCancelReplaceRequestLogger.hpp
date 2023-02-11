// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderCancelReplaceRequestLogger_h__
#define __OrderCancelReplaceRequestLogger_h__

#include "OrderCancelReplaceRequestTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class OrderCancelReplaceRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderCancelReplaceRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.OrigClOrdID : " << ddsMsg.OrigClOrdID << std::endl
			 << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID << std::endl
			 << "ddsMsg.ExecInst : " << ddsMsg.ExecInst << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
			 << "ddsMsg.Side : " << ddsMsg.Side << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime << std::endl
			 << "ddsMsg.OrderQty : " << ddsMsg.OrderQty << std::endl
			 << "ddsMsg.OrdType : " << ddsMsg.OrdType << std::endl
			 << "ddsMsg.Price : " << ddsMsg.Price << std::endl
			 << "ddsMsg.StopPx : " << ddsMsg.StopPx << std::endl
			 << "ddsMsg.TimeInForce : " << ddsMsg.TimeInForce << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

