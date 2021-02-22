// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderMassStatusRequestLogger_h__
#define __OrderMassStatusRequestLogger_h__

#include "OrderMassStatusRequestTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class OrderMassStatusRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassStatusRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.MassStatusReqID : " << ddsMsg.MassStatusReqID << std::endl
			 << "ddsMsg.MassStatusReqType : " << ddsMsg.MassStatusReqType << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

