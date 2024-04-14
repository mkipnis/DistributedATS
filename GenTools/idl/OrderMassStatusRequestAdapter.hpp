/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderMassStatusRequest.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class OrderMassStatusRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};