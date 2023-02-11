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

#include "Market.h"

#include "OrderException.h"
#include <cctype>
#include <functional>
#include <locale>

#include <ExecutionReportLogger.hpp>
#include <LoggerHelper.h>
#include <MarketDataRequestLogger.hpp>
#include <OrderCancelRejectLogger.hpp>
#include <OrderMassCancelReportLogger.hpp>
#include <SecurityListRequestLogger.hpp>

namespace DistributedATS {

uint32_t Market::orderIdSeed_ = 0;
uint32_t Market::executionIdSeed_ = 1000000;

Market::Market(DataWriterContainerPtr dataWriterContainerPtr,
               std::string marketName, std::string dataServiceName,
               PriceDepthPublisherQueuePtr&
               price_depth_publisher_queue_ptr)
    : dataWriterContainerPtr_(dataWriterContainerPtr), _marketName(marketName), _dataServiceName(dataServiceName),
    _price_depth_publisher_queue_ptr( price_depth_publisher_queue_ptr )
{
  stats_ptr_ = std::make_shared<OrderBookStatsMap>();
}

Market::~Market() {}

void Market::populatePriceLevelDepth(
    const std::string &symbol, DistributedATS::Market *market,
    const DistributedATS::BookDepth &depth,
                                     DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
        *marketDataRefresh) {

    DistributedATS_MarketDataIncrementalRefresh::NoMDEntries mdEntry;
  mdEntry.Symbol = CORBA::string_dup(symbol.c_str());
  mdEntry.SecurityExchange = CORBA::string_dup(market->getMarketName().c_str());
  mdEntry.MDUpdateAction = '0';

  uint16_t md_index = 0;

  auto pos = depth.bids();

  while (pos != depth.end()) {
    mdEntry.MDEntrySize = pos->aggregate_qty();

    if (mdEntry.MDEntrySize == 0)
      mdEntry.MDEntryPx = 0;
    else
      mdEntry.MDEntryPx = pos->price();

    mdEntry.MDEntrySize = pos->aggregate_qty();
    mdEntry.MDEntryType = md_index < MARKET_DATA_PRICE_DEPTH ? FIX::MDEntryType_BID
                                                     : FIX::MDEntryType_OFFER;

    marketDataRefresh->c_NoMDEntries[md_index++] = mdEntry;

    pos++;
  };
}

bool Market::submit(const OrderBookPtr &book, const OrderPtr &order) {

  auto contra_party_orders = orders_.find(order->contra_party());

  OrderMapPtr customerOrders;

  if (contra_party_orders == orders_.end()) {
    customerOrders.reset(new OrderMap());
    orders_.emplace(order->contra_party(), customerOrders);
  } else {
    customerOrders = contra_party_orders->second;
  }

  auto existing_order = customerOrders->find(order->order_id());
  if (existing_order != customerOrders->end()) {
    throw DistributedATS::OrderException(order->contra_party(),
                                         order->order_id(),
                                         FIX::OrdRejReason_DUPLICATE_ORDER);
  }

  customerOrders->emplace(order->order_id(), order);

  //const liquibook::book::OrderConditions NOC(liquibook::book::oc_no_conditions);
  book->add(order, order->conditions());

  return true;
}

void Market::publishSecurityListRequest() {
    DistributedATS_SecurityListRequest::SecurityListRequest securityListRequest;
  securityListRequest.m_Header.TargetCompID =
      CORBA::string_dup("REF_DATA_SERVICE");
  securityListRequest.m_Header.SenderCompID =
      CORBA::string_dup(getMarketName().c_str());
  securityListRequest.m_Header.TargetSubID =
      CORBA::string_dup(_dataServiceName.c_str());

  LoggerHelper::log_debug<std::stringstream, SecurityListRequestLogger,
    DistributedATS_SecurityListRequest::SecurityListRequest>(
      securityListRequest, "SecurityListRequest");

  int ret = dataWriterContainerPtr_->_security_list_request_dw->write(securityListRequest, NULL);

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR(
        (LM_ERROR,
         ACE_TEXT("(%P|%t) ERROR: SecurityListRequest write returned %d.\n"),
         ret));
  }
}

void Market::publishMarketDataRequest() {
    DistributedATS_MarketDataRequest::MarketDataRequest marketDataRequest;
  marketDataRequest.m_Header.TargetCompID = CORBA::string_dup("DATA_SERVICE");
  marketDataRequest.m_Header.SenderCompID =
      CORBA::string_dup(getMarketName().c_str());
  marketDataRequest.m_Header.TargetSubID =
      CORBA::string_dup(_dataServiceName.c_str());

  marketDataRequest.c_NoMDEntryTypes.length(1);
  marketDataRequest.c_NoMDEntryTypes[0].MDEntryType =
      FIX::MDEntryType_TRADE; // Last trade price

  marketDataRequest.c_NoRelatedSym.length((int)books_.size());

  int index = 0;
  for (auto &book : books_) {
    std::cout << "Need To Publish Market Data Request for Book" << book.first
              << std::endl;
    marketDataRequest.c_NoRelatedSym[index].Symbol =
        CORBA::string_dup(book.first.c_str());
    marketDataRequest.c_NoRelatedSym[index].SecurityExchange =
        CORBA::string_dup(getMarketName().c_str());

    index++;
  }

  LoggerHelper::log_debug<std::stringstream, MarketDataRequestLogger,
    DistributedATS_MarketDataRequest::MarketDataRequest>(
      marketDataRequest, "MarketDataRequest");

  int ret = dataWriterContainerPtr_->_market_date_request_dw->write(marketDataRequest, NULL);

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR(
        (LM_ERROR,
         ACE_TEXT("(%P|%t) ERROR: MarketDataRequest write returned %d.\n"),
         ret));
  }
}

bool Market::cancel_order(const OrderBookPtr &book,
                          const std::string &counter_party,
                          const std::string &client_order_id) {
  auto contra_party_orders = orders_.find(counter_party);

  if (contra_party_orders == orders_.end()) {
    std::cerr << "Order for given counter party not found : " << counter_party
              << std::endl;

    return false;
  };

  auto orderIter = contra_party_orders->second->find(client_order_id);

  if (orderIter == contra_party_orders->second->end()) {
    throw DistributedATS::OrderException(counter_party, client_order_id,
                                         FIX::OrdRejReason_UNKNOWN_ORDER);
  }

  book->cancel(orderIter->second);

  return true;
}


/// @brief attempts to replace existing order
bool Market::replace_order(const OrderBookPtr &book, const std::string &counter_party,
                           const std::string &orig_client_order_id, const std::string& client_order_id,
                            int32_t size_delta, liquibook::book::Price new_price)
{
    auto contra_party_orders = orders_.find(counter_party);

    if (contra_party_orders == orders_.end()) {
      std::cerr << "Order for given counter party not found : " << counter_party
                << std::endl;

      return false;
    };

    auto orderIter = contra_party_orders->second->find(orig_client_order_id);

    if (orderIter == contra_party_orders->second->end()) {
      throw DistributedATS::OrderException(counter_party, client_order_id,
                                           FIX::OrdRejReason_UNKNOWN_ORDER);
    }
    
    orderIter->second->onReplaceRequested(client_order_id, size_delta, new_price);
    book->replace(orderIter->second, size_delta, new_price);
    
    contra_party_orders->second->emplace(client_order_id, orderIter->second );
    contra_party_orders->second->erase(orig_client_order_id);
    
    return true;
};

bool Market::mass_cancel(const std::string &counter_party) {
  auto contra_party_orders = orders_.find(counter_party);

  if (contra_party_orders == orders_.end()) {
    std::cerr << "Orders for given counter party not found : " << counter_party
              << std::endl;

    return false;
  };

  size_t order_to_be_cancelled = contra_party_orders->second->size();

  for (auto orderIter : *contra_party_orders->second) {
    OrderPtr order = orderIter.second;

    auto book = findBook(order->symbol());

    // book->log(out());
    book->cancel(order);
    // book->log(out());
  }

  return true;
}

OrderBookPtr Market::addBook(const std::string &symbol, bool useDepthBook) {
  OrderBookPtr result;
  if (useDepthBook) {
    // out() << "Create new depth order book for " << symbol << std::endl;
    DepthOrderBookPtr depthBook = std::make_shared<DepthOrderBook>(symbol);
    depthBook->set_bbo_listener(this);
    depthBook->set_depth_listener(this);
    result = depthBook;
  } else {
    // out() << "Create new order book for " << symbol << std::endl;
    result = std::make_shared<OrderBook>(symbol);
  }
  result->set_order_listener(this);
  result->set_trade_listener(this);
  result->set_order_book_listener(this);
  books_[symbol] = result;
  return result;
}

OrderBookPtr Market::findBook(const std::string &symbol) {
  OrderBookPtr result;
  auto entry = books_.find(symbol);
  if (entry != books_.end()) {
    result = entry->second;
  }
  return result;
}

void Market::publishExecutionReport(
                                    DistributedATS_ExecutionReport::ExecutionReport &executionReport) {
  executionReport.ExecID = getNextExecutionReportID().c_str();

  LoggerHelper::log_debug<std::stringstream, ExecutionReportLogger,
    DistributedATS_ExecutionReport::ExecutionReport>(
      executionReport, "ExecutionReport");

  int ret = dataWriterContainerPtr_->_execution_report_dw->write(executionReport, NULL);

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("(%P|%t) ERROR: Execution Report write returned %d.\n"),
               ret));
  }
}

void Market::publishOrderMassCancelReport(
                                          DistributedATS_OrderMassCancelReport::OrderMassCancelReport &orderMassCancelReport) {

  LoggerHelper::log_debug<std::stringstream, OrderMassCancelReportLogger,
    DistributedATS_OrderMassCancelReport::OrderMassCancelReport>(
      orderMassCancelReport, "OrderMassCancelReport");

  int ret = dataWriterContainerPtr_->_order_mass_cancel_report_dw->write(orderMassCancelReport, NULL);

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR(
        (LM_ERROR,
         ACE_TEXT(
             "(%P|%t) ERROR: Order Mass Cancel Report write returned %d.\n"),
         ret));
  }
}

void Market::publishOrderCancelReject(
                                      DistributedATS_OrderCancelReject::OrderCancelReject &orderCancelReject) {

  LoggerHelper::log_debug<std::stringstream, OrderCancelRejectLogger,
    DistributedATS_OrderCancelReject::OrderCancelReject>(
      orderCancelReject, "OrderCancelReject");

  int ret = dataWriterContainerPtr_->_order_cancel_reject_dw->write(orderCancelReject, NULL);

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR(
        (LM_ERROR,
         ACE_TEXT("(%P|%t) ERROR: Order Cancel Reject write returned %d.\n"),
         ret));
  }
}

/////////////////////////////////////
// Implement OrderListener interface

void Market::on_accept(const OrderPtr &order) {
  order->onAccepted();

    DistributedATS_ExecutionReport::ExecutionReport executionReport;
  order->populateExecutionReport(executionReport, FIX::ExecType_NEW);

  publishExecutionReport(executionReport);
}

void Market::on_reject(const OrderPtr &order, const char *reason) {
  order->onRejected(reason);

    DistributedATS_ExecutionReport::ExecutionReport executionReport;

  order->populateExecutionReport(executionReport, FIX::ExecType_REJECTED);

  publishExecutionReport(executionReport);
}

void Market::on_fill(const OrderPtr &order, const OrderPtr &matched_order,
                     liquibook::book::Quantity fill_qty,
                     liquibook::book::Cost fill_cost) {
  order->onFilled(fill_qty, fill_cost);
  matched_order->onFilled(fill_qty, fill_cost);

    DistributedATS_ExecutionReport::ExecutionReport executionReport;
    DistributedATS_ExecutionReport::ExecutionReport executionReportMatched;

  if (order->quantityOnMarket() == 0)
    order->populateExecutionReport(executionReport, FIX::ExecType_FILL);
  else
    order->populateExecutionReport(executionReport, FIX::ExecType_PARTIAL_FILL);

  if (matched_order->quantityOnMarket() == 0)
    matched_order->populateExecutionReport(executionReportMatched,
                                           FIX::ExecType_FILL);
  else
    matched_order->populateExecutionReport(executionReportMatched,
                                           FIX::ExecType_PARTIAL_FILL);

  executionReport.LastPx = executionReportMatched.LastPx = fill_cost / fill_qty;
  executionReport.LastQty = executionReportMatched.LastQty = fill_qty;

  publishExecutionReport(executionReport);
  publishExecutionReport(executionReportMatched);
}

void Market::on_cancel(const OrderPtr &order) {
  order->onCancelled();
    DistributedATS_ExecutionReport::ExecutionReport executionReport;
  order->populateExecutionReport(executionReport, FIX::ExecType_CANCELLED);
  publishExecutionReport(executionReport);

  // out() << "\tCanceled: " << *order<< std::endl;
}

void Market::on_cancel_reject(const OrderPtr &order, const char *reason) {
  order->onCancelRejected(reason);
  // out() << "\tCancel Reject: " <<*order<< " : " << reason << std::endl;
}

void Market::on_replace(const OrderPtr &order, const int32_t &size_delta,
                        liquibook::book::Price new_price) {
  // out() << "\tCancel Reject: " <<*order<< " : " << size_delta << " : "  <<
  // new_price << std::endl;
    order->onReplaced(size_delta, new_price);
    DistributedATS_ExecutionReport::ExecutionReport executionReport;
    order->populateExecutionReport(executionReport, FIX::ExecType_REPLACE);
    publishExecutionReport(executionReport);
}

void Market::on_replace_reject(const OrderPtr &order, const char *reason) {
    order->onReplaceRejected(reason);
  // out() << "\tCancel Reject: " <<*order<< " : " << reason << std::endl;
}

void Market::on_trade(const OrderBook *book, liquibook::book::Quantity qty,
                      liquibook::book::Cost cost) {
  auto price = book->market_price();
  update_symbol_stats(book, price, qty);
}

void Market::on_order_book_change(const OrderBook *book) {}

void Market::on_bbo_change(const DepthOrderBook *book, const BookDepth *depth) {
}

void Market::on_depth_change(const DepthOrderBook *book,
                             const BookDepth *depth) {
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
      *marketDataRefresh =
          new DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh();

  marketDataRefresh->m_Header.SenderCompID =
      CORBA::string_dup("MATCHING_ENGINE");
  marketDataRefresh->m_Header.MsgType = CORBA::string_dup("X");

  marketDataRefresh->c_NoMDEntries.length(15);

  populatePriceLevelDepth(book->symbol(), this, *depth, marketDataRefresh);

  int market_data_index = MARKET_DATA_PRICE_DEPTH * 2; // bids and asks

  auto current_stats = stats_ptr_->find(book->symbol());

  // populating stats
  if (current_stats != stats_ptr_->end()) {
    if (current_stats->second->volume > 0) {
      // Last traded price and traded volume
        DistributedATS_MarketDataIncrementalRefresh::NoMDEntries mdEntry;

      set_market_data_stats_entry(
          marketDataRefresh->c_NoMDEntries[market_data_index++],
          getMarketName(), book->symbol(), FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADE, book->market_price(), 0);

      set_market_data_stats_entry(
          marketDataRefresh->c_NoMDEntries[market_data_index++],
          getMarketName(), book->symbol(), FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADE_VOLUME, 0, current_stats->second->volume);

      set_market_data_stats_entry(
          marketDataRefresh->c_NoMDEntries[market_data_index++],
          getMarketName(), book->symbol(), FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_OPENING_PRICE, current_stats->second->open, 0);

      set_market_data_stats_entry(
          marketDataRefresh->c_NoMDEntries[market_data_index++],
          getMarketName(), book->symbol(), FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADING_SESSION_LOW_PRICE,
          current_stats->second->low, 0);

      set_market_data_stats_entry(
          marketDataRefresh->c_NoMDEntries[market_data_index++],
          getMarketName(), book->symbol(), FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADING_SESSION_HIGH_PRICE,
          current_stats->second->high, 0);

    } else {
      set_market_data_stats_entry(
          marketDataRefresh->c_NoMDEntries[market_data_index++],
          getMarketName(), book->symbol(), FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_OPENING_PRICE, current_stats->second->open, 0);
    }
  }

  market_data_update *mdUpdate = new market_data_update();
  mdUpdate->symbol = book->symbol();
  mdUpdate->priceDepth = marketDataRefresh;

  ACE_Message_Block *msg =
      new ACE_Message_Block((char *)mdUpdate, sizeof(mdUpdate));

  _price_depth_publisher_queue_ptr->enqueue_tail(msg);
}

void Market::cancel_all_orders() {
  for (auto counter_party_iter : orders_) {
    for (auto orderIter : *counter_party_iter.second) {
      OrderPtr order = orderIter.second;

      auto book = findBook(order->symbol());

      book->cancel(order);
    }
  }
};

void Market::set_market_price(const std::string &symbol,
                              liquibook::book::Price price) {
  auto book = findBook(symbol);

  if (book != NULL) {
    std::cout << "Setting market price : " << symbol << ":" << price
              << std::endl;

    update_symbol_stats(book.get(), price, 0);

    book->set_market_price(price);

  } else {
    std::cerr << "Book not found" << std::endl;
  }
};

void Market::update_symbol_stats(const OrderBook *book,
                                 liquibook::book::Price price,
                                 liquibook::book::Quantity qty) {
  auto symbol_stats = stats_ptr_->find(book->symbol());

  if (symbol_stats == stats_ptr_->end()) {
    auto stats = std::make_shared<DistributedATS::OrderBookStats>();
    stats->volume = qty;
    stats->open = stats->low = stats->high = price;

    stats_ptr_->emplace(book->symbol(), stats);
  } else {
    symbol_stats->second->volume += qty;

    if (symbol_stats->second->low > price)
      symbol_stats->second->low = price;
    else if (symbol_stats->second->high < price)
      symbol_stats->second->high = price;
  }
}

void Market::set_market_data_stats_entry(
                                         DistributedATS_MarketDataIncrementalRefresh::NoMDEntries &mdEntry,
    const std::string &market, const std::string &symbol, const char md_action,
    const char md_entry_type, const float MDEntryPx, const float MDEntrySize) {

  mdEntry.SecurityExchange = CORBA::string_dup(getMarketName().c_str());
  mdEntry.Symbol = CORBA::string_dup(symbol.c_str());
  mdEntry.MDUpdateAction = md_action;
  mdEntry.MDEntryType = md_entry_type;

  if (md_entry_type == FIX::MDEntryType_TRADE_VOLUME)
    mdEntry.MDEntrySize = MDEntrySize;
  else
    mdEntry.MDEntryPx = MDEntryPx;
}

} // namespace DistributedATS
