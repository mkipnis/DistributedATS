/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "ExecutionReport.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class ExecutionReportAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedATS_ExecutionReport::ExecutionReport& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedATS_ExecutionReport::ExecutionReport& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};