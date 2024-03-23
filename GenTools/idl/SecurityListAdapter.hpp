/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "SecurityList.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class SecurityListAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_SecurityList::SecurityList& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_SecurityList::SecurityList& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};