/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class OrderMassStatusRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassStatusRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
			 << "ddsMsg.MassStatusReqID : " << ddsMsg.MassStatusReqID() << std::endl
			 << "ddsMsg.MassStatusReqType : " << ddsMsg.MassStatusReqType() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
;			out << "}";
		out << std::endl;};

};
