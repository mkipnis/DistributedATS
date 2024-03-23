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
       eprosima::fastdds::dds::DataReader* reader)
{
    
    DistributedATS_NewOrderSingle::NewOrderSingle new_order_single;
    eprosima::fastdds::dds::SampleInfo info;
        
    if (reader->take_next_sample(&new_order_single, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            
            LoggerHelper::log_debug<std::stringstream, NewOrderSingleLogger,
            DistributedATS_NewOrderSingle::NewOrderSingle>( logger,
                                                           new_order_single, "NewOrderSingle");
            
            std::string symbol = new_order_single.Symbol();
            std::string securityExchange = new_order_single.SecurityExchange();
            
            try {
                auto book = _market->findBook(symbol);
                
                if (!book) {
                    throw DistributedATS::OrderException(
                                                         new_order_single, FIX::OrdRejReason_UNKNOWN_SYMBOL);
                }
                
                std::string clientOrderId = new_order_single.ClOrdID();
                
                bool buy_side = false;
                
                if (new_order_single.Side() == '1')
                    buy_side = true;
                
                std::string gateway = new_order_single.header().SenderCompID();
                std::string dataService = new_order_single.header().TargetSubID();
                std::string contra_party = new_order_single.header().SenderSubID();
                auto quantity = new_order_single.OrderQty();
                auto price = new_order_single.Price();
                auto stop_price = new_order_single.StopPx();
                
                liquibook::book::OrderCondition order_condition = liquibook::book::OrderCondition::oc_no_conditions;
                
                // ExecInst is string in the data dictionary but char in quickfix/c++
                // <field number='18' name='ExecInst' type='MULTIPLEVALUESTRING'>
                if ( new_order_single.ExecInst().length() > 0 && new_order_single.ExecInst()[0] == FIX::ExecInst_ALL_OR_NONE )
                {
                    order_condition = liquibook::book::OrderCondition::oc_all_or_none;
                }
                
                if ( new_order_single.TimeInForce() == FIX::TimeInForce_IMMEDIATE_OR_CANCEL)
                {
                    order_condition = liquibook::book::OrderCondition::oc_immediate_or_cancel;
                } else if ( new_order_single.TimeInForce() == FIX::TimeInForce_FILL_OR_KILL )
                {
                    order_condition = liquibook::book::OrderCondition::oc_fill_or_kill;
                }
                
                DistributedATS::OrderPtr order =
                std::make_shared<DistributedATS::Order>(
                                                        _market->getDataWriterContainer(),
                                                        gateway, dataService, contra_party, clientOrderId, buy_side,
                                                        quantity, symbol, securityExchange, price, stop_price, order_condition);
                
                order->onSubmitted();
                
                _market->submit(book, order);
                
            } catch (DistributedATS::OrderException &orderException) {
                DistributedATS_ExecutionReport::ExecutionReport executionReport;
                executionReport.header().SenderCompID("MATCHING_ENGINE");
                executionReport.header().SenderSubID(new_order_single.header().SenderSubID());
                executionReport.header().TargetCompID(new_order_single.header().SenderCompID());
                executionReport.header().TargetSubID(new_order_single.header().TargetSubID());
                executionReport.header().MsgType("8");
                executionReport.Symbol(symbol);
                executionReport.SecurityExchange(securityExchange);
                executionReport.Side(new_order_single.Side());
                executionReport.Text("Order Rejected");
                executionReport.CumQty(0);
                executionReport.LeavesQty(0);
                executionReport.Price(0);
                executionReport.TimeInForce(new_order_single.TimeInForce());
                executionReport.OrdType(new_order_single.OrdType());
                executionReport.ExecInst(new_order_single.ExecInst());
                
                orderException.populateExecutionReportWithRejectCode(executionReport);
                
                _market->publishExecutionReport(executionReport);
            }
        }
    }
}

} // namespace MatchingEngine
