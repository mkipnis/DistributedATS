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

#include "OrderMassCancelReportDataReaderListenerImpl.h"
#include "Application.hpp"
#include <OrderMassCancelReportAdapter.hpp>
#include <OrderMassCancelReportLogger.hpp>

#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

namespace DistributedATS {

auto const order_mass_cencel_report_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_OrderMassCancelReport::OrderMassCancelReport& orderMassCancelReport)
{
    FIX::Message orderMassCancelReportMessage;

    orderMassCancelReport.header().BeginString("FIX.4.4");
    orderMassCancelReport.header().SendingTime(0); // this is precision;
    orderMassCancelReport.header().SenderCompID(
        orderMassCancelReport.header().TargetSubID());

    OrderMassCancelReportAdapter::DDS2FIX(orderMassCancelReport,
                                          orderMassCancelReportMessage);

    DistributedATS::DATSApplication::publishToClient(
        orderMassCancelReportMessage);
};


OrderMassCancelReportDataReaderListenerImpl::OrderMassCancelReportDataReaderListenerImpl(DistributedATS::DATSApplication &application) :
    _processor(application, order_mass_cencel_report_processor, "OrderMassCancelReportDataReaderListenerImpl", 500 )
{};

void OrderMassCancelReportDataReaderListenerImpl::on_data_available(
                            eprosima::fastdds::dds::DataReader* reader) {
  
    DistributedATS_OrderMassCancelReport::OrderMassCancelReport orderMassCancelReport;
    eprosima::fastdds::dds::SampleInfo info;
    if (reader->take_next_sample(&orderMassCancelReport, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            _processor.enqueue_dds_message(orderMassCancelReport);
        }
    }
        
}

} /* namespace DistributedATS */
