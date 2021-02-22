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

#include "NewOrderSingleDataReaderListenerImpl.h"
#include "OrderException.h"
#include <iostream>
#include <vector>

#include <quickfix/FixValues.h>

#include <LoggerHelper.h>
#include <NewOrderSingleLogger.hpp>

#include <BusinessMessageRejectTypeSupportImpl.h>

namespace MatchingEngine {

NewOrderSingleDataReaderListenerImpl::NewOrderSingleDataReaderListenerImpl(
    std::shared_ptr<DistributedATS::Market> market)
    : _market(market) {
  // TODO Auto-generated constructor stub
}

NewOrderSingleDataReaderListenerImpl::~NewOrderSingleDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub

} /* namespace MatchingEngine */

void NewOrderSingleDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_NewOrderSingle::NewOrderSingleDataReader_var new_order_single_dr =
      DistributedATS_NewOrderSingle::NewOrderSingleDataReader::_narrow(reader);

    if (CORBA::is_nil(new_order_single_dr.in())) {
      std::cerr << "NewOrderSingleDataReaderListenerImpl::on_data_available: "
                   "_narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_NewOrderSingle::NewOrderSingle new_order_single;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          new_order_single_dr->take_next_sample(new_order_single, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

        LoggerHelper::log_debug<std::stringstream, NewOrderSingleLogger,
          DistributedATS_NewOrderSingle::NewOrderSingle>(
            new_order_single, "NewOrderSingle");

        std::string symbol = new_order_single.Symbol.in();
        std::string securityExchange = new_order_single.SecurityExchange.in();

        try {
          auto book = _market->findBook(symbol);

          if (!book) {
            throw DistributedATS::OrderException(
                new_order_single, FIX::OrdRejReason_UNKNOWN_SYMBOL);
          }

          std::string clientOrderId = new_order_single.ClOrdID.in();

          bool buy_side = false;

          if (new_order_single.Side == '1')
            buy_side = true;

          std::string gateway = new_order_single.m_Header.SenderCompID.in();
          std::string dataService = new_order_single.m_Header.TargetSubID.in();
          std::string contra_party = new_order_single.m_Header.SenderSubID.in();
          auto quantity = new_order_single.OrderQty;
          auto price = new_order_single.Price;

          DistributedATS::OrderPtr order =
              std::make_shared<DistributedATS::Order>(
                  gateway, dataService, contra_party, clientOrderId, buy_side,
                  quantity, symbol, securityExchange, price, 0, false, false);

          order->onSubmitted();

          _market->submit(book, order);

        } catch (DistributedATS::OrderException &orderException) {
            DistributedATS_ExecutionReport::ExecutionReport executionReport;
          executionReport.m_Header.SenderCompID =
              CORBA::string_dup("MATCHING_ENGINE");
          executionReport.m_Header.SenderSubID =
              CORBA::string_dup(new_order_single.m_Header.SenderSubID);
          executionReport.m_Header.TargetCompID =
              CORBA::string_dup(new_order_single.m_Header.SenderCompID);
          executionReport.m_Header.TargetSubID =
              CORBA::string_dup(new_order_single.m_Header.TargetSubID);
          executionReport.m_Header.MsgType = CORBA::string_dup("8");
          executionReport.Symbol = CORBA::string_dup(symbol.c_str());
          executionReport.SecurityExchange =
              CORBA::string_dup(securityExchange.c_str());
          executionReport.Side = new_order_single.Side;
          executionReport.Text = "Order Rejected";
          executionReport.CumQty = 0;
          executionReport.LeavesQty = 0;
          executionReport.Price = 0;

          orderException.populateExecutionReportWithRejectCode(executionReport);

          _market->publishExecutionReport(executionReport);
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

} // namespace MatchingEngine
