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

#include "Order.h"
#include <iostream>
#include <sstream>

#include <LoggerHelper.h>
#include <OrderCancelRejectLogger.hpp>

#include <math.h>

namespace DistributedATS {

Order::Order( DataWriterContainerPtr dataWriterContainerPtr,
             const std::string &gateway, const std::string &dataService,
             const std::string &contra_party,
             const std::string &client_order_id, bool buy_side,
             liquibook::book::Quantity quantity, std::string &symbol,
             std::string &securityExchange, liquibook::book::Price price,
             liquibook::book::Price stopPrice, liquibook::book::OrderConditions conditions)
    : dataWriterContainerPtr_(dataWriterContainerPtr), gateway_(gateway),
      dataService_(dataService), contra_party_(contra_party),
      client_order_id_(client_order_id), buy_side_(buy_side), symbol_(symbol),
      securityExchange_(securityExchange), quantity_(quantity), price_(price),
      stopPrice_(stopPrice), conditions_(conditions), quantityFilled_(0),
      quantityOnMarket_(0), fillCost_(0), verbose_(false)

{}

std::string Order::order_id() const { return client_order_id_; }

std::string Order::contra_party() const { return contra_party_; }

bool Order::is_limit() const { return price() != 0; }

bool Order::is_buy() const { return buy_side_; }

bool Order::all_or_none() const
{
    return (conditions_ & liquibook::book::OrderCondition::oc_all_or_none) != 0;
}

bool Order::immediate_or_cancel() const
{
    return (conditions_ & liquibook::book::OrderCondition::oc_immediate_or_cancel) != 0;
}

liquibook::book::OrderConditions Order::conditions() const
{
    return conditions_;
}

std::string Order::symbol() const { return symbol_; }

liquibook::book::Price Order::price() const { return price_; }

liquibook::book::Quantity Order::order_qty() const { return quantity_; }

liquibook::book::Price Order::stop_price() const { return stopPrice_; }

uint32_t Order::quantityOnMarket() const { return quantityOnMarket_; }

uint32_t Order::quantityFilled() const { return quantityFilled_; }

uint32_t Order::fillCost() const { return fillCost_; }

Order &Order::verbose(bool verbose) {
  verbose_ = verbose;
  return *this;
}

bool Order::isVerbose() const { return verbose_; }

void Order::onSubmitted() {}

void Order::onAccepted() { quantityOnMarket_ = quantity_; }

void Order::onRejected(const char *reason) {}

void Order::onFilled(liquibook::book::Quantity fill_qty,
                     liquibook::book::Cost fill_cost) {
  quantityOnMarket_ -= fill_qty;
  fillCost_ += fill_cost;
  quantityFilled_ += fill_qty;

  std::stringstream msg;
  msg << fill_qty << " for " << fill_cost;
}

void Order::onCancelRequested() {}

void Order::onCancelled() { quantityOnMarket_ = 0; }

void Order::onCancelRejected(const char *reason)
{
    DistributedATS_OrderCancelReject::OrderCancelReject orderCancelReject;
    
    orderCancelReject.header().TargetSubID(dataService_);
    orderCancelReject.header().TargetCompID(gateway_);
    orderCancelReject.header().SenderSubID(contra_party_);
    
    orderCancelReject.header().MsgType("9");
    orderCancelReject.Text(reason);
    orderCancelReject.ClOrdID(client_order_id_);
    
    LoggerHelper::log_debug<std::stringstream, OrderCancelRejectLogger,
    DistributedATS_OrderCancelReject::OrderCancelReject>(logger,
                                                         orderCancelReject, "OrderCancelReject");

    int ret = dataWriterContainerPtr_->order_cancel_reject_dw->write(&orderCancelReject);

    if (ret != eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK) {
        LOG4CXX_ERROR(logger, "OrderCancelReject write returned :" << ret);
    }
    
}

void Order::onReplaceRequested(const std::string& replacement_client_order_id,
                               const int32_t &size_delta,
                               liquibook::book::Price new_price) {
   std::stringstream msg;
   if(size_delta != liquibook::book::SIZE_UNCHANGED)
   {
       msg << "Quantity change: " << size_delta << ' ';
   }
   if(new_price != liquibook::book::PRICE_UNCHANGED)
   {
       msg << "New Price " << new_price;
   }
    
    replacement_client_order_id_  = replacement_client_order_id;
}

void Order::onReplaced(const int32_t &size_delta,
                       liquibook::book::Price new_price) {
  std::stringstream msg;
  if(size_delta != liquibook::book::SIZE_UNCHANGED)
  {
      quantity_ += size_delta;
      quantityOnMarket_ += size_delta;
      msg << "Quantity change: " << size_delta << ' ';
  }
  if(new_price != liquibook::book::PRICE_UNCHANGED)
  {
      price_ = new_price;
      msg << "New Price " << new_price;
  }
  client_order_id_ = replacement_client_order_id_;
}

void Order::onReplaceRejected(const char *reason)
{
    DistributedATS_OrderCancelReject::OrderCancelReject orderCancelReject;
    
    orderCancelReject.header().TargetSubID(dataService_);
    orderCancelReject.header().TargetCompID(gateway_);
    orderCancelReject.header().SenderSubID(contra_party_);
    
    orderCancelReject.header().MsgType("9");
    orderCancelReject.Text(reason);
    orderCancelReject.ClOrdID(client_order_id_);
    
    LoggerHelper::log_debug<std::stringstream, OrderCancelRejectLogger,
    DistributedATS_OrderCancelReject::OrderCancelReject>( logger,
                                                         orderCancelReject, "OrderCancelReject");

    int ret = dataWriterContainerPtr_->order_cancel_reject_dw->write(&orderCancelReject);
    
    if (ret != eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK) {
        LOG4CXX_ERROR(logger, "OrdereplaceRejected write returned :" << ret);
    }
}

void Order::populateExecutionReport(
                                    DistributedATS_ExecutionReport::ExecutionReport &executionReport,
                                    char ExecType)
    {
            
  executionReport.header().SenderCompID("MATCHING_ENGINE");
  executionReport.header().TargetSubID(dataService_);
  executionReport.header().TargetCompID(gateway_);
  executionReport.header().SenderSubID(contra_party_);
  executionReport.header().MsgType("8");
  executionReport.OrderID(client_order_id_);
  executionReport.Side(is_buy() ? '1' : '2');
  executionReport.Symbol(symbol());
  executionReport.SecurityExchange(securityExchange_);
  executionReport.ExecType(ExecType);
  executionReport.CumQty(quantityFilled());
  executionReport.LeavesQty(quantityOnMarket());
  executionReport.Price(price());
  executionReport.StopPx(stop_price());
  
  if ( stop_price() !=0 )
      executionReport.OrdType(FIX::OrdType_STOP);
  else if ( price() !=0 )
      executionReport.OrdType(FIX::OrdType_LIMIT);
  else
      executionReport.OrdType(FIX::OrdType_MARKET);

  if ( all_or_none() )
      executionReport.ExecInst("G");
      
  executionReport.TransactTime(
    std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count());
    
  executionReport.LastPx(0);
  executionReport.LastQty(0);

  if ( conditions_ == liquibook::book::OrderCondition::oc_immediate_or_cancel )
  {
      executionReport.TimeInForce(FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
  } else if ( conditions_ == liquibook::book::OrderCondition::oc_fill_or_kill )
  {
      executionReport.TimeInForce(FIX::TimeInForce_FILL_OR_KILL);
  } else {
      executionReport.TimeInForce(FIX::TimeInForce_DAY);
  }


  // std::cout << "Transact Time : " << ACE_OS::gettimeofday().msec() << ":" <<
  // executionReport.TransactTime << std::endl;

  if (quantityFilled() > 0)
    executionReport.AvgPx(std::nearbyint(fillCost() / quantityFilled())); // round to the nearest tick
  else
    executionReport.AvgPx(0); // avoid scientific numbers

  executionReport.OrderQty(order_qty());
  executionReport.OrdStatus(ExecType);
  executionReport.OrdRejReason(0);
  executionReport.Text("OK");
}

std::ostream &operator<<(std::ostream &out, const Order &order) {
  out << "[#" << order.order_id();
  out << ' ' << (order.is_buy() ? "BUY" : "SELL");
  out << ' ' << order.order_qty();
  out << ' ' << order.symbol();
  if (order.price() == 0) {
    out << " MKT";
  } else {
    out << " $" << order.price();
  }

  if (order.stop_price() != 0) {
    out << " STOP " << order.stop_price();
  }

  out << (order.all_or_none() ? " AON" : "")
      << (order.immediate_or_cancel() ? " IOC" : "");

  auto onMarket = order.quantityOnMarket();
  if (onMarket != 0) {
    out << " Open: " << onMarket;
  }

  auto filled = order.quantityFilled();
  if (filled != 0) {
    out << " Filled: " << filled;
  }

  auto cost = order.fillCost();
  if (cost != 0) {
    out << " Cost: " << cost;
  }

  out << ']';

  return out;
}

} // namespace DistributedATS
