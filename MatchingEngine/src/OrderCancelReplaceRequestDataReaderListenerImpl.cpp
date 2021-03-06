/*
   Copyright (C) 2021 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over OpenDDS. This project simplifies
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

#include "OrderCancelReplaceRequestDataReaderListenerImpl.h"

#include "OrderException.h"
#include <OrderCancelRejectTypeSupportImpl.h>

#include <LoggerHelper.h>
#include <OrderCancelReplaceRequestLogger.hpp>

namespace MatchingEngine {

OrderCancelReplaceRequestDataReaderListenerImpl::
OrderCancelReplaceRequestDataReaderListenerImpl(
        std::shared_ptr<DistributedATS::Market> market)
    : _market(market) {
  // TODO Auto-generated constructor stub
}

OrderCancelReplaceRequestDataReaderListenerImpl::
    ~OrderCancelReplaceRequestDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
}

void OrderCancelReplaceRequestDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestDataReader_var
        order_cancel_replace_request_dr =
      DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestDataReader::_narrow(
                reader);

    if (CORBA::is_nil(order_cancel_replace_request_dr.in())) {
      std::cerr << "OrderCancelReplaceRequestDataReaderListenerImpl::on_data_"
                   "available: _narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest order_cancel_replace_request;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          order_cancel_replace_request_dr->take_next_sample(order_cancel_replace_request, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

        LoggerHelper::log_debug<std::stringstream, OrderCancelReplaceRequestLogger,
          DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest>(
                                        order_cancel_replace_request, "OrderCancelReplaceRequest");

        std::string symbol = order_cancel_replace_request.Symbol.in();

        try {
          auto book = _market->findBook(symbol);

          if (!book) {
            std::cerr << "Order book not found to cancel order : " << symbol
                      << std::endl;
            break;
          }

          std::string orig_client_order_id = order_cancel_replace_request.OrigClOrdID.in();
          std::string client_order_id = order_cancel_replace_request.ClOrdID.in();
          std::string contra_party =
            order_cancel_replace_request.m_Header.SenderSubID.in();

          _market->replace_order(book, contra_party,
                                 orig_client_order_id,
                                 client_order_id,
                                 order_cancel_replace_request.OrderQty,
                                 order_cancel_replace_request.Price);

        } catch (DistributedATS::OrderException &orderException) {
            DistributedATS_OrderCancelReject::OrderCancelReject orderCancelReject;
          orderCancelReject.m_Header.SenderCompID =
              CORBA::string_dup("MATCHING_ENGINE");
          orderCancelReject.m_Header.TargetCompID =
              CORBA::string_dup(order_cancel_replace_request.m_Header.SenderCompID);
          orderCancelReject.m_Header.MsgType = CORBA::string_dup("9");
          orderCancelReject.Text = "Cancel Replace Rejected";
          orderCancelReject.ClOrdID = order_cancel_replace_request.ClOrdID;

          _market->publishOrderCancelReject(orderCancelReject);
        }

      } else if (status == DDS::RETCODE_NO_DATA) {
        break;
      } else {
        std::cerr << "ERROR: read DATS::Logon: Error: " << status << std::endl;
      }
    }

  } catch (CORBA::Exception &e) {
    std::cerr << "Exception caught in read:" << std::endl << e << std::endl;
    ACE_OS::exit(1);
  }
}

} /* namespace MatchingEngine */

