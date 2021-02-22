// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderCancelReject_h__
#define __OrderCancelReject_h__

#include "OrderCancelRejectTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class OrderCancelRejectAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderCancelReject::OrderCancelReject& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderCancelReject::OrderCancelReject& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

