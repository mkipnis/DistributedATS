/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class OrderMassCancelReportLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassCancelReport { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
			 << "ddsMsg.OrderID : " << ddsMsg.OrderID() << std::endl
			 << "ddsMsg.MassCancelRequestType : " << ddsMsg.MassCancelRequestType() << std::endl
			 << "ddsMsg.MassCancelResponse : " << ddsMsg.MassCancelResponse() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
