// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __ExecutionReport_h__
#define __ExecutionReport_h__

#include "ExecutionReportTypeSupportImpl.h"
#include <quickfix/Message.h>

 using namespace DistributedATS;

#include "HeaderAdapter.hpp"


class ExecutionReportAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_ExecutionReport::ExecutionReport& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_ExecutionReport::ExecutionReport& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};
#endif

