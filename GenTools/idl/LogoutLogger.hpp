// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __LogoutLogger_h__
#define __LogoutLogger_h__

#include "LogoutTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class LogoutLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_Logout::Logout& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : Logout { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
			 << "ddsMsg.Text : " << ddsMsg.Text << std::endl
;			out << "}";
		out << std::endl;};

};

#endif

