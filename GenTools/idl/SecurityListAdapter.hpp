// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __SecurityList_h__
#define __SecurityList_h__

#include "SecurityListTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class SecurityListAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_SecurityList::SecurityList& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_SecurityList::SecurityList& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

