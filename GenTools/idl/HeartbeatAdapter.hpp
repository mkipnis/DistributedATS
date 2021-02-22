// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __Heartbeat_h__
#define __Heartbeat_h__

#include "HeartbeatTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class HeartbeatAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_Heartbeat::Heartbeat& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_Heartbeat::Heartbeat& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

