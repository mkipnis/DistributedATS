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

#include "OrderMassCancelRequestDataReaderListenerImpl.h"
#include <LoggerHelper.h>
#include <OrderMassCancelRequestLogger.hpp>


namespace MatchingEngine {

OrderMassCancelRequestDataReaderListenerImpl::
    OrderMassCancelRequestDataReaderListenerImpl(
        std::shared_ptr<DistributedATS::Market> market)
    : _market(market) {
  // TODO Auto-generated constructor stub
}

OrderMassCancelRequestDataReaderListenerImpl::
    ~OrderMassCancelRequestDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
}

void OrderMassCancelRequestDataReaderListenerImpl::on_data_available(
       eprosima::fastdds::dds::DataReader* reader)
{

    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest
          order_mass_cancel_request;
    eprosima::fastdds::dds::SampleInfo info;

          
          if (reader->take_next_sample(&order_mass_cancel_request, &info) == eprosima::fastdds::dds::RETCODE_OK)
          {
               if (info.valid_data)
               {

        LoggerHelper::log_debug<
            std::stringstream, OrderMassCancelRequestLogger,
          DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest>(logger,
            order_mass_cancel_request, "OrderMassCancelRequest");

        std::string contra_party =
                   order_mass_cancel_request.DATS_SourceUser();

          DistributedATS_OrderMassCancelReport::OrderMassCancelReport
            order_mass_cancel_report;
                   
        order_mass_cancel_report.DATS_Source("MATCHING_ENGINE");
        order_mass_cancel_report.DATS_Destination(order_mass_cancel_request.DATS_Source());
        order_mass_cancel_report.DATS_DestinationUser(order_mass_cancel_request.DATS_SourceUser());
        
        order_mass_cancel_report.fix_header().MsgType("r");
        order_mass_cancel_report.MassCancelRequestType(FIX::MassCancelRequestType_CANCEL_ALL_ORDERS);
        order_mass_cancel_report.MassCancelResponse(FIX::MassCancelResponse_CANCEL_ALL_ORDERS);
        order_mass_cancel_report.OrderID(order_mass_cancel_request.ClOrdID());

        _market->publishOrderMassCancelReport(order_mass_cancel_report);

        _market->mass_cancel(contra_party);

      } 
  }
}

} // namespace MatchingEngine
