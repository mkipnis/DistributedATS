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

			;out 
			 << "ddsMsg.DATS_Source : " << ddsMsg.DATS_Source() << std::endl 
			 << "ddsMsg.DATS_Destination : " << ddsMsg.DATS_Destination() << std::endl
			 << "ddsMsg.DATS_SourceUser : " << ddsMsg.DATS_SourceUser() << std::endl
			 << "ddsMsg.DATS_DestinationUser : " << ddsMsg.DATS_DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
