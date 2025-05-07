/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "Logon.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class LogonAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_Logon::Logon& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_Logon::Logon& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};