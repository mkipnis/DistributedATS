/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class LogoutLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_Logout::Logout& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : Logout { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
