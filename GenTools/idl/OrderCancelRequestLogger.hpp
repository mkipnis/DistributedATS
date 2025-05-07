/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class OrderCancelRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderCancelRequest::OrderCancelRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderCancelRequest { " << std::endl;

			;out 
			 << "ddsMsg.DATS_Source : " << ddsMsg.DATS_Source() << std::endl 
			 << "ddsMsg.DATS_Destination : " << ddsMsg.DATS_Destination() << std::endl
			 << "ddsMsg.DATS_SourceUser : " << ddsMsg.DATS_SourceUser() << std::endl
			 << "ddsMsg.DATS_DestinationUser : " << ddsMsg.DATS_DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.OrigClOrdID : " << ddsMsg.OrigClOrdID() << std::endl
			 << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
			 << "ddsMsg.Side : " << ddsMsg.Side() << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
			 << "ddsMsg.OrderQty : " << ddsMsg.OrderQty() << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
