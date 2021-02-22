// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __SecurityListRequestLogger_h__
#define __SecurityListRequestLogger_h__

#include "SecurityListRequestTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class SecurityListRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : SecurityListRequest { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.SecurityReqID : " << ddsMsg.SecurityReqID << std::endl
			 << "ddsMsg.SecurityListRequestType : " << ddsMsg.SecurityListRequestType << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

