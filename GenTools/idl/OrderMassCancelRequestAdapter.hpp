/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderMassCancelRequest.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class OrderMassCancelRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};