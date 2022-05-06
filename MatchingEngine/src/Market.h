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

#pragma once

#include <book/depth_order_book.h>

#include "Order.h"

#include <ExecutionReportTypeSupportImpl.h>
#include <MarketDataIncrementalRefreshTypeSupportImpl.h>
#include <MarketDataRequestTypeSupportImpl.h>
#include <OrderCancelRejectTypeSupportImpl.h>
#include <OrderMassCancelReportTypeSupportImpl.h>
#include <SecurityListRequestTypeSupportImpl.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <ace/Message_Queue_T.h>

#include "OrderBookStats.h"

#include "DataWriterContainer.h"


namespace DistributedATS {

struct market_data_update {
  std::string symbol;
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
      *priceDepth;
};

typedef ACE_Message_Queue<ACE_MT_SYNCH> PriceDepthPublisherQueue;
typedef std::shared_ptr<PriceDepthPublisherQueue> PriceDepthPublisherQueuePtr;

typedef liquibook::book::OrderBook<OrderPtr> OrderBook;
typedef std::shared_ptr<OrderBook> OrderBookPtr;
typedef liquibook::book::DepthOrderBook<OrderPtr> DepthOrderBook;
typedef std::shared_ptr<DepthOrderBook> DepthOrderBookPtr;
typedef liquibook::book::Depth<> BookDepth;

// MARKET_DATA_ORDERBOOK_DEPTH is used for market data updates only
// see $LIQUIDBOOK_HOME/src/book/depth_order_book.h for the actual order book depth
const int MARKET_DATA_PRICE_DEPTH = 5;

class Market : public liquibook::book::OrderListener<OrderPtr>,
               public liquibook::book::TradeListener<OrderBook>,
               public liquibook::book::OrderBookListener<OrderBook>,
               public liquibook::book::BboListener<DepthOrderBook>,
               public liquibook::book::DepthListener<DepthOrderBook> {
  typedef std::map<std::string, OrderPtr> OrderMap;
  typedef std::shared_ptr<OrderMap> OrderMapPtr;
  typedef std::map<std::string, OrderMapPtr> CustomerOrderMap;
  typedef std::map<std::string, OrderBookPtr> SymbolToBookMap;
  typedef std::map<std::string, DistributedATS::OrderBookStatsPtr>
      OrderBookStatsMap;
  typedef std::shared_ptr<OrderBookStatsMap> OrderBookStatsMapPtr;

public:
  Market(DataWriterContainerPtr dataWriterContainerPtr,
         std::string marketName, std::string dataServiceName,
         PriceDepthPublisherQueuePtr &price_depth_publisher_queue_ptr);
  virtual ~Market();
 
  /// @brief submits new order single
  bool submit(const OrderBookPtr &, const OrderPtr &order);

  /// @brief attempts to cancel existing order
  bool cancel_order(const OrderBookPtr &book, const std::string &counter_party,
                    const std::string &client_order_id);

  /// @brief mass cancels all orders from a given FIX Client/counter_party, called on clients disconnects as well
  bool mass_cancel(const std::string &counter_party);
                   
    /// @brief attempts to replace existing order
 bool replace_order(const OrderBookPtr &book, const std::string &counter_party,
                                     const std::string &orig_client_order_id, const std::string& client_order_id,
                                int32_t size_delta, liquibook::book::Price new_price);

  /// @brief sets opening market price
  void set_market_price(const std::string &symbol,
                        liquibook::book::Price price);
  /// @brief updated statistics i.e. open/close/high/low for a given order OrderBook
  void update_symbol_stats(const OrderBook *book, liquibook::book::Price price,
                           liquibook::book::Quantity qty);

  /// @brief cancells all orders
  void cancel_all_orders();
  
  /// @brief looks up order boook for a given symbol
  OrderBookPtr findBook(const std::string &symbol);

  /// @brief published SecurityListRequest to DataService, in order to recieve a list of instruments to setup
  void publishSecurityListRequest();

  /// @brief publishes MarketDataRequest to DataService, in order to recieve opening prices for received securies on the SecurityListRequest channel
  void publishMarketDataRequest(); // Market Data Request

  /// @brief Logical name of the MachingEngine on DDS domain, i.e. its SENDER_COMP_ID
  std::string getMarketName() { return _marketName; };

  /// @brief builds price depth snapshot that will be published as an MarketDataIncrementalRefresh(X) to FIX Gateways and then clients, and to DataServices
  void populatePriceLevelDepth(
      const std::string &symbol, DistributedATS::Market *market,
      const DistributedATS::BookDepth &depth,
            DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
          *marketDataRefresh);

public:
  //
  // Implementation of LiquiBook interfaces
  //
  // OrderListener interface
  virtual void on_accept(const OrderPtr &order);
  virtual void on_reject(const OrderPtr &order, const char *reason);
  virtual void on_fill(const OrderPtr &order, const OrderPtr &matched_order,
                       liquibook::book::Quantity fill_qty,
                       liquibook::book::Cost fill_cost);
  virtual void on_cancel(const OrderPtr &order);
  virtual void on_cancel_reject(const OrderPtr &order, const char *reason);
  virtual void on_replace(const OrderPtr &order, const int32_t &size_delta,
                          liquibook::book::Price new_price);
  virtual void on_replace_reject(const OrderPtr &order, const char *reason);

  // TradeListener interface
  virtual void on_trade(const OrderBook *book, liquibook::book::Quantity qty,
                        liquibook::book::Cost cost);

  // OrderBookListener interface
  virtual void on_order_book_change(const OrderBook *book);

  // BboListener interface
  void on_bbo_change(const DepthOrderBook *book, const BookDepth *depth);

  // Implement DepthListener interface
  void on_depth_change(const DepthOrderBook *book, const BookDepth *depth);

  OrderBookPtr addBook(const std::string &symbol, bool useDepthBook);

public:
  /* void setExecutionReportDataWriter(
    DistributedATS_ExecutionReport::ExecutionReportDataWriter_var &execution_report_dw) {
    _execution_report_dw = execution_report_dw;
  }

  void setMarketDataIncrementalRefreshDataWriter(
        DistributedATS_MarketDataIncrementalRefresh::
          MarketDataIncrementalRefreshDataWriter_var
              &marketdata_incremental_refresh_dw) {
    _marketdata_incremental_refresh_dw = marketdata_incremental_refresh_dw;
  }

  void setOrderCancelRejectDataWriter(
                                      DistributedATS_OrderCancelReject::OrderCancelRejectDataWriter_var
          &order_cancel_reject_dw) {
    _order_cancel_reject_dw = order_cancel_reject_dw;
  }

  void setOrderMassCancelReportDataWriter(
        DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataWriter_var
          &order_mass_cancel_report_dw) {
    _order_mass_cancel_report_dw = order_mass_cancel_report_dw;
  }

  void setSecurityListRequestDataWriter(
    DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_var
          &security_list_request_dw) {
    _security_list_request_dw = security_list_request_dw;
  }

  void setMarketDataRequestDataWriter(
        DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_var
          &market_date_request_dw) {
    _market_date_request_dw = market_date_request_dw;
  }*/
                   
  DataWriterContainerPtr getDataWriterContainer()
  {
      return dataWriterContainerPtr_;
  }

  void
  publishExecutionReport(DistributedATS_ExecutionReport::ExecutionReport &executionReport);
  void publishOrderCancelReject(
    DistributedATS_OrderCancelReject::OrderCancelReject &orderCancelReject);
  void publishOrderMassCancelReport(
        DistributedATS_OrderMassCancelReport::OrderMassCancelReport &orderMassCancelReport);

private:
  void set_market_data_stats_entry(
    DistributedATS_MarketDataIncrementalRefresh::NoMDEntries &mdEntry,
      const std::string &market, const std::string &symbol,
      const char md_action, const char md_entry_type, const float MDEntryPx,
      const float MDEntrySize);

  std::string getNextExecutionReportID() {
    char execution_report_id_c[64];
    sprintf(execution_report_id_c, "%d", ++executionIdSeed_);
    std::string execution_report_id = execution_report_id_c;

    return execution_report_id;
  };

private:
  static uint32_t orderIdSeed_;
  static uint32_t executionIdSeed_;

  PriceDepthPublisherQueuePtr _price_depth_publisher_queue_ptr;

  CustomerOrderMap orders_;
  SymbolToBookMap books_;
  OrderBookStatsMapPtr stats_ptr_;
  DataWriterContainerPtr dataWriterContainerPtr_;

  std::string _marketName; // Quickfix field 207 - Security Exchange
  std::string _dataServiceName;
};

typedef std::shared_ptr<Market> MarketPtr;

} // namespace DistributedATS
