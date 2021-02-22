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

#include "OrderException.h"

namespace DistributedATS {

OrderException::OrderException(
    const DistributedATS_NewOrderSingle::NewOrderSingle &newOrderSingle, const int reason)
    : _client_order_id(newOrderSingle.ClOrdID.in()),
      _counter_partry(newOrderSingle.m_Header.SenderCompID.in()),
      _reason(reason) {
  // TODO Auto-generated constructor stub
}

OrderException::OrderException(const std::string &counter_party,
                               const std::string &client_order_id,
                               const int reason)
    : _client_order_id(client_order_id), _counter_partry(counter_party),
      _reason(reason) {}

OrderException::~OrderException() {
  // TODO Auto-generated destructor stub
}

bool OrderException::populateExecutionReportWithRejectCode(
                                                           DistributedATS_ExecutionReport::ExecutionReport &executionReport) {
  executionReport.OrderID = CORBA::string_dup(_client_order_id.c_str());
  executionReport.ExecType = FIX::ExecType_REJECTED;
  executionReport.OrdStatus = FIX::OrdStatus_REJECTED;
  executionReport.OrdRejReason = _reason;

  return true;
}

bool OrderException::populateOrderCancelRejectWithReasonCode(
                                                             DistributedATS_OrderCancelReject::OrderCancelReject &cancelReject) {
  cancelReject.CxlRejResponseTo = _reason;
  return true;
}

} /* namespace DistributedATS */
