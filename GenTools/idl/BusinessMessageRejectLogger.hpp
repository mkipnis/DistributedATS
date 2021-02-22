// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __BusinessMessageRejectLogger_h__
#define __BusinessMessageRejectLogger_h__

#include "BusinessMessageRejectTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class BusinessMessageRejectLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_BusinessMessageReject::BusinessMessageReject& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : BusinessMessageReject { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.RefMsgType : " << ddsMsg.RefMsgType << std::endl
			 << "ddsMsg.BusinessRejectReason : " << ddsMsg.BusinessRejectReason << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

