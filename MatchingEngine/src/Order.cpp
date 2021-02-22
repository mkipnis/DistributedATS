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

#include "Order.h"
#include <iostream>
#include <sstream>

namespace DistributedATS {

Order::Order(const std::string &gateway, const std::string &dataService,
             const std::string &contra_party,
             const std::string &client_order_id, bool buy_side,
             liquibook::book::Quantity quantity, std::string &symbol,
             std::string &securityExchange, liquibook::book::Price price,
             liquibook::book::Price stopPrice, bool aon, bool ioc)
    : gateway_(gateway), dataService_(dataService), contra_party_(contra_party),
      client_order_id_(client_order_id), buy_side_(buy_side), symbol_(symbol),
      securityExchange_(securityExchange), quantity_(quantity), price_(price),
      stopPrice_(stopPrice), ioc_(ioc), aon_(aon), quantityFilled_(0),
      quantityOnMarket_(0), fillCost_(0), verbose_(false)

{}

std::string Order::order_id() const { return client_order_id_; }

std::string Order::contra_party() const { return contra_party_; }

bool Order::is_limit() const { return price() != 0; }

bool Order::is_buy() const { return buy_side_; }

bool Order::all_or_none() const { return aon_; }

bool Order::immediate_or_cancel() const { return ioc_; }

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

void Order::onCancelRejected(const char *reason) {}

void Order::onReplaceRequested(const int32_t &size_delta,
                               liquibook::book::Price new_price) {
  /* std::stringstream msg;
   if(size_delta != liquibook::book::SIZE_UNCHANGED)
   {
       msg << "Quantity change: " << size_delta << ' ';
   }
   if(new_price != liquibook::book::PRICE_UNCHANGED)
   {
       msg << "New Price " << new_price;
   }*/
}

void Order::onReplaced(const int32_t &size_delta,
                       liquibook::book::Price new_price) {
  /*std::stringstream msg;
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
  }*/
}

void Order::onReplaceRejected(const char *reason) {}

void Order::populateExecutionReport(
                                    DistributedATS_ExecutionReport::ExecutionReport &executionReport, char ExecType) {

  executionReport.m_Header.SenderCompID = CORBA::string_dup("MATCHING_ENGINE");
  executionReport.m_Header.TargetSubID =
      CORBA::string_dup(dataService_.c_str());
  executionReport.m_Header.TargetCompID = CORBA::string_dup(gateway_.c_str());
  executionReport.m_Header.SenderSubID =
      CORBA::string_dup(contra_party_.c_str());
  executionReport.m_Header.MsgType = CORBA::string_dup("8");
  executionReport.OrderID = CORBA::string_dup(client_order_id_.c_str());
  executionReport.Side = (is_buy() ? '1' : '2');
  executionReport.Symbol = CORBA::string_dup(symbol().c_str());
  executionReport.SecurityExchange =
      CORBA::string_dup(securityExchange_.c_str());
  executionReport.ExecType = ExecType;
  executionReport.CumQty = quantityFilled();
  executionReport.LeavesQty = quantityOnMarket();
  executionReport.Price = price();
  executionReport.TransactTime =
      (ACE_OS::gettimeofday().sec() * 1000000) + ACE_OS::gettimeofday().usec();
  executionReport.LastPx = 0;
  executionReport.LastQty = 0;

  // std::cout << "Transact Time : " << ACE_OS::gettimeofday().msec() << ":" <<
  // executionReport.TransactTime << std::endl;

  if (quantityFilled() > 0)
    executionReport.AvgPx = fillCost() / quantityFilled();

  executionReport.OrderQty = order_qty();
  executionReport.OrdStatus = ExecType;
  executionReport.OrdRejReason = 0;
  executionReport.Text = "OK";
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
