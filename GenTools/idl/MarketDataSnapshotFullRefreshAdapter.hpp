/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "MarketDataSnapshotFullRefresh.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class MarketDataSnapshotFullRefreshAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};