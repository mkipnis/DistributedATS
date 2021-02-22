// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __MarketDataRequest_h__
#define __MarketDataRequest_h__

#include "MarketDataRequestTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class MarketDataRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

