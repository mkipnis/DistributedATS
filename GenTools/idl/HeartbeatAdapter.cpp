/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "HeartbeatAdapter.hpp"
#include <ConvertUtils.h>


void HeartbeatAdapter::FIX2DDS(const FIX::Message& fixMsg, DistributedATS_Heartbeat::Heartbeat& ddsMsg )
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.header());


};



void HeartbeatAdapter::DDS2FIX(const DistributedATS_Heartbeat::Heartbeat& ddsMsg, FIX::Message& fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.header(), fixMsg.getHeader());


};

