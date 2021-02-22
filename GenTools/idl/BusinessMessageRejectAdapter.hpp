// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __BusinessMessageReject_h__
#define __BusinessMessageReject_h__

#include "BusinessMessageRejectTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class BusinessMessageRejectAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_BusinessMessageReject::BusinessMessageReject& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_BusinessMessageReject::BusinessMessageReject& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

