// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderCancelRequest_h__
#define __OrderCancelRequest_h__

#include "OrderCancelRequestTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class OrderCancelRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderCancelRequest::OrderCancelRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderCancelRequest::OrderCancelRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

