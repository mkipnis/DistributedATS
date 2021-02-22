// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __Logout_h__
#define __Logout_h__

#include "LogoutTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class LogoutAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_Logout::Logout& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_Logout::Logout& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

