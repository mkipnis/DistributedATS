/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "Heartbeat.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class HeartbeatAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_Heartbeat::Heartbeat& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_Heartbeat::Heartbeat& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};