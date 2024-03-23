/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class HeartbeatLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_Heartbeat::Heartbeat& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : Heartbeat { " << std::endl;
			HeaderLogger::log(out, ddsMsg.header());


			;out
;			out << "}";
		out << std::endl;};

};
