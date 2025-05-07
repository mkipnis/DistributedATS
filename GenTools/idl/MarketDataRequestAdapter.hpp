/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "MarketDataRequest.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class MarketDataRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_MarketDataRequest::MarketDataRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};