/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "MarketDataIncrementalRefresh.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class MarketDataIncrementalRefreshAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};