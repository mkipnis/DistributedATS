// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __OrderMassStatusRequest_h__
#define __OrderMassStatusRequest_h__

#include "OrderMassStatusRequestTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class OrderMassStatusRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

