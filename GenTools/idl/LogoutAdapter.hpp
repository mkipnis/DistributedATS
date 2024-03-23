/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "Logout.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class LogoutAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_Logout::Logout& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_Logout::Logout& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};