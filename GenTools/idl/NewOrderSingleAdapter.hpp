// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __NewOrderSingle_h__
#define __NewOrderSingle_h__

#include "NewOrderSingleTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class NewOrderSingleAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_NewOrderSingle::NewOrderSingle& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_NewOrderSingle::NewOrderSingle& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

