/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "Header.h"
#include <quickfix/Message.h>



class HeaderAdapter
{
	public:
		static void FIX2DDS(const FIX::FieldMap&, DistributedATS::Header& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS::Header& ddsMsg, FIX::FieldMap&)  __attribute__ ((visibility ("default")));

};