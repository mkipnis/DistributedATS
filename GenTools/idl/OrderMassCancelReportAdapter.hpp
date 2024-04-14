/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderMassCancelReport.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class OrderMassCancelReportAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderMassCancelReport::OrderMassCancelReport& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};