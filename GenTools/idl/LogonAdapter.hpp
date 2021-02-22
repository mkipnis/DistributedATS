// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __Logon_h__
#define __Logon_h__

#include "LogonTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class LogonAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_Logon::Logon& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_Logon::Logon& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

