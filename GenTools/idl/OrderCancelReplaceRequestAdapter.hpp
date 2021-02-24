// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderCancelReplaceRequest_h__
#define __OrderCancelReplaceRequest_h__

#include "OrderCancelReplaceRequestTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class OrderCancelReplaceRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

