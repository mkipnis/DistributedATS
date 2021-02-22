// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __LogonLogger_h__
#define __LogonLogger_h__

#include "LogonTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class LogonLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_Logon::Logon& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : Logon { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.EncryptMethod : " << ddsMsg.EncryptMethod << std::endl
			 << "ddsMsg.HeartBtInt : " << ddsMsg.HeartBtInt << std::endl
			 << "ddsMsg.RawData : " << ddsMsg.RawData << std::endl
			 << "ddsMsg.Username : " << ddsMsg.Username << std::endl
			 << "ddsMsg.Password : " << ddsMsg.Password << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

