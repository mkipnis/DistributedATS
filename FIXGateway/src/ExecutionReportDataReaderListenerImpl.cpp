/*
   Copyright (C) 2021 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include <ExecutionReportAdapter.hpp>
#include <quickfix/Session.h>
#include "ExecutionReportDataReaderListenerImpl.h"
#include <ExecutionReportLogger.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

#include "Application.hpp"

auto const exec_report_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_ExecutionReport::ExecutionReport& executionReport)
{
    FIX::Message executionReportMessage;

    executionReport.fix_header().BeginString("FIX.4.4");
    executionReport.fix_header().TargetCompID(executionReport.DATS_Destination());
    executionReport.fix_header().SenderCompID(executionReport.DATS_DestinationUser());
    executionReport.fix_header().SendingTime(0);
    
    ExecutionReportAdapter::DDS2FIX(executionReport, executionReportMessage);

    DistributedATS::DATSApplication::publishToClient(executionReportMessage);
};


namespace DistributedATS {

ExecutionReportDataReaderListenerImpl::ExecutionReportDataReaderListenerImpl(DistributedATS::DATSApplication &application)
    : _processor(application, exec_report_processor, "ExecutionReportDataReaderListenerImpl", 100)
{
}

void ExecutionReportDataReaderListenerImpl::on_data_available(
                    eprosima::fastdds::dds::DataReader* reader)
{
    DistributedATS_ExecutionReport::ExecutionReport executionReport;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&executionReport, &info) == eprosima::fastdds::dds::RETCODE_OK)
    {
        if (info.valid_data)
        {
            _processor.enqueue_dds_message(executionReport);
        }
    }
    
}

} // namespace DistributedATS
