/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "SecurityListRequest.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class SecurityListRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_SecurityListRequest::SecurityListRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};