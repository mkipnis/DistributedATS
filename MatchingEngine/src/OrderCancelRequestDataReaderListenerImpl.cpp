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

#include "OrderCancelRequestDataReaderListenerImpl.h"

#include "OrderException.h"

#include <LoggerHelper.h>
#include <OrderCancelRequestLogger.hpp>


namespace MatchingEngine {

OrderCancelRequestDataReaderListenerImpl::
    OrderCancelRequestDataReaderListenerImpl(
        std::shared_ptr<DistributedATS::Market> market)
    : _market(market) {
  // TODO Auto-generated constructor stub
}

OrderCancelRequestDataReaderListenerImpl::
    ~OrderCancelRequestDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
}

void OrderCancelRequestDataReaderListenerImpl::on_data_available(
      eprosima::fastdds::dds::DataReader* reader) {
    
    
    DistributedATS_OrderCancelRequest::OrderCancelRequest
    order_cancel_request;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&order_cancel_request, &info) == eprosima::fastdds::dds::RETCODE_OK)
    {
         if (info.valid_data)
         {
             LoggerHelper::log_debug<std::stringstream, OrderCancelRequestLogger,
               DistributedATS_OrderCancelRequest::OrderCancelRequest>(logger,
                 order_cancel_request, "OrderCancelRequest");

             std::string symbol = order_cancel_request.Symbol();

             try {
               auto book = _market->findBook(symbol);

               if (!book) {
                 std::cerr << "Order book not found to cancel order : " << symbol
                           << std::endl;
               }

               std::string client_order_id = order_cancel_request.OrigClOrdID();
               std::string contra_party =
                   order_cancel_request.DATS_SourceUser();

               _market->cancel_order(book, contra_party, client_order_id);

             } catch (DistributedATS::OrderException &orderException) {
                 DistributedATS_OrderCancelReject::OrderCancelReject orderCancelReject;
               orderCancelReject.DATS_Source("MATCHING_ENGINE");
               orderCancelReject.DATS_Destination(order_cancel_request.DATS_Source());
               orderCancelReject.fix_header().MsgType("9");
               orderCancelReject.Text("Cancel Rejected REJECT");
               orderCancelReject.ClOrdID(order_cancel_request.ClOrdID());

               _market->publishOrderCancelReject(orderCancelReject);
             }
         }
     }
}

} /* namespace MatchingEngine */
