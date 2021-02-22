// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __HeartbeatLogger_h__
#define __HeartbeatLogger_h__

#include "HeartbeatTypeSupportImpl.h"
#include "HeaderLogger.hpp"
;


class HeartbeatLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_Heartbeat::Heartbeat& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : Heartbeat { " << std::endl;
			HeaderLogger::log(out, ddsMsg.m_Header);


			;out
;			out << "}";
		out << std::endl;};

};

#endif

