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
#include "OrderCancelRejectDataReaderListenerImpl.h"
#include "Application.hpp"
#include <OrderCancelRejectAdapter.hpp>
#include <OrderCancelRejectLogger.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

namespace DistributedATS {

auto const order_cancel_reject_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_OrderCancelReject::OrderCancelReject& orderCancelReject)
{

    FIX::Message orderCancelRejectMessage;

    orderCancelReject.fix_header().SendingTime(0); // this is precision;
    HeaderAdapter::DDS2FIX(orderCancelReject.fix_header(),
                           orderCancelRejectMessage.getHeader());
    OrderCancelRejectAdapter::DDS2FIX(orderCancelReject,
                                      orderCancelRejectMessage);

    DistributedATS::DATSApplication::publishToClient(
        orderCancelRejectMessage);
};

OrderCancelRejectDataReaderListenerImpl::OrderCancelRejectDataReaderListenerImpl(DistributedATS::DATSApplication &application)
    :_processor(application, order_cancel_reject_processor, "OrderCancelRejectDataReaderListenerImpl", 100 )
{
    
};

void OrderCancelRejectDataReaderListenerImpl::on_data_available(
        eprosima::fastdds::dds::DataReader* reader)
{
    
    DistributedATS_OrderCancelReject::OrderCancelReject orderCancelReject;
    eprosima::fastdds::dds::SampleInfo info;
     if (reader->take_next_sample(&orderCancelReject, &info) == eprosima::fastdds::dds::RETCODE_OK)
     {
         if (info.valid_data)
         {
             _processor.enqueue_dds_message(orderCancelReject);
         }
     }
  
}

} /* namespace DistributedATS */
