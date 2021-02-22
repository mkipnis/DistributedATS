// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __Header_h__
#define __Header_h__

#include "HeaderTypeSupportImpl.h"
#include <quickfix/Message.h>

using namespace DistributedATS;



class HeaderAdapter
{
	public:
		static void FIX2DDS(const FIX::FieldMap&, DistributedATS::Header& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS::Header& ddsMsg, FIX::FieldMap&)  __attribute__ ((visibility ("default")));

};
#endif