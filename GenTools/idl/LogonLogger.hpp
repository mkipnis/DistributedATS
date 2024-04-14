/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class LogonLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_Logon::Logon& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : Logon { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
			 << "ddsMsg.EncryptMethod : " << ddsMsg.EncryptMethod() << std::endl
			 << "ddsMsg.HeartBtInt : " << ddsMsg.HeartBtInt() << std::endl
			 << "ddsMsg.RawData : " << ddsMsg.RawData() << std::endl
			 << "ddsMsg.Username : " << ddsMsg.Username() << std::endl
			 << "ddsMsg.Password : " << ddsMsg.Password() << std::endl
;			out << "}";
		out << std::endl;};

};
