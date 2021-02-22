// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderMassCancelReport_h__
#define __OrderMassCancelReport_h__

#include "OrderMassCancelReportTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class OrderMassCancelReportAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

