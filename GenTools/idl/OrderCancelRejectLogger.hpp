/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class OrderCancelRejectLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderCancelReject::OrderCancelReject& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderCancelReject { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
			 << "ddsMsg.OrderID : " << ddsMsg.OrderID() << std::endl
			 << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID() << std::endl
			 << "ddsMsg.OrigClOrdID : " << ddsMsg.OrigClOrdID() << std::endl
			 << "ddsMsg.OrdStatus : " << ddsMsg.OrdStatus() << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
			 << "ddsMsg.CxlRejResponseTo : " << ddsMsg.CxlRejResponseTo() << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
