/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderCancelReplaceRequest.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class OrderCancelReplaceRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};