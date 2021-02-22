// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __SecurityListRequest_h__
#define __SecurityListRequest_h__

#include "SecurityListRequestTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class SecurityListRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

