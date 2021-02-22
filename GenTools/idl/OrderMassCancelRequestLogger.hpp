// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderMassCancelRequestLogger_h__
#define __OrderMassCancelRequestLogger_h__

#include "OrderMassCancelRequestTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class OrderMassCancelRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassCancelRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID << std::endl
			 << "ddsMsg.MassCancelRequestType : " << ddsMsg.MassCancelRequestType << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

