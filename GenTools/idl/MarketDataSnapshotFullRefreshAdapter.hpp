// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __MarketDataSnapshotFullRefresh_h__
#define __MarketDataSnapshotFullRefresh_h__

#include "MarketDataSnapshotFullRefreshTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class MarketDataSnapshotFullRefreshAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

