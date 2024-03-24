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

#pragma once

#include <book/types.h>

#include <ExecutionReport.h>
#include <OrderCancelReject.h>
#include <memory>
#include <quickfix/FixValues.h>
#include <string>
#include <vector>

#include "DataWriterContainer.h"

namespace DistributedATS {

class Order {
public:
  Order(DataWriterContainerPtr dataWriterContainerPtr,
        const std::string
            &gateway, // name of the gateway from which order was originated, it
                      // will be used as a destination for execution reports
        const std::string
            &data_service, // Name of the associated with FIX Gateway Data
                           // Service, Data Service will filter in executions
                           // reports so it can service mass status requests
        const std::string &contra_party, // Sender Comp ID, so FIX Gateway know
                                         // whom to reply to
        const std::string
            &client_order_id, // client order id from new order single
        bool buy_side, liquibook::book::Quantity quantity, std::string &symbol,
        std::string &security_exchange, liquibook::book::Price price,
        liquibook::book::Price stopPrice,
        liquibook::book::OrderConditions conditions = liquibook::book::OrderCondition::oc_no_conditions );

  //////////////////////////
  // Implement the
  // liquibook::book::order
  // concept.
  /// @brief is this a limit order?
  bool is_limit() const;

  /// @brief is this order a buy?
  bool is_buy() const;

  /// @brief get the price of this order, or 0 if a market order
  liquibook::book::Price price() const;

  /// @brief get the stop price (if any) for this order.
  /// @returns the stop price or zero if not a stop order
  liquibook::book::Price stop_price() const;

  /// @brief get the quantity of this order
  liquibook::book::Quantity order_qty() const;

  /// @brief if no trades should happen until the order
  /// can be filled completely.
  /// Note: one or more trades may be used to fill the order.
  virtual bool all_or_none() const;

  /// @brief After generating as many trades as possible against
  /// orders already on the market, cancel any remaining quantity.
  virtual bool immediate_or_cancel() const;
    
  virtual liquibook::book::OrderConditions conditions() const;
  ///////////////////////////////////////////////////

  /// @brief instrument name
  std::string symbol() const;

  /// @brief Client OrderID
  std::string order_id() const;

  /// @brief FIX Target CompID
  std::string contra_party() const;

  ///////////////////////////
  // Order life cycle events
  void onSubmitted();
  void onAccepted();
  void onRejected(const char *reason);
  void onFilled(liquibook::book::Quantity fill_qty,
                liquibook::book::Cost fill_cost);
  void onCancelRequested();
  void onCancelled();
  void onCancelRejected(const char *reason);
  void onReplaceRequested(const std::string& replacement_client_order_id, const int32_t &size_delta,
                          liquibook::book::Price new_price);
  void onReplaced(const int32_t &size_delta, liquibook::book::Price new_price);
  void onReplaceRejected(const char *reason);
  
  uint32_t quantityFilled() const;
  uint32_t quantityOnMarket() const;
  uint32_t fillCost() const;
  Order &verbose(bool verbose = true);
  bool isVerbose() const;

  ///////////////////
  // Execution Report
  void
  populateExecutionReport(DistributedATS_ExecutionReport::ExecutionReport &executionReport,
                          const char ExecType);
    ////////
    // Cancel Reject
    /*void
    populateCancelReject(DistributedATS_OrderCancelReject::OrderCancelReject &orderCancelReject);
     */

private:
  std::string client_order_id_;
  std::string contra_party_;
  std::string gateway_;
  std::string dataService_;
  bool buy_side_;
  std::string symbol_;
  std::string securityExchange_;
  liquibook::book::Quantity quantity_;
  liquibook::book::Price price_;
  liquibook::book::Price stopPrice_;

  liquibook::book::OrderConditions conditions_;

  liquibook::book::Quantity quantityFilled_;
  int32_t quantityOnMarket_;
  uint32_t fillCost_;

  // std::vector<StateChange> history_;
  bool verbose_;

  std::string replacement_client_order_id_;
    
  DataWriterContainerPtr dataWriterContainerPtr_;
};

typedef std::shared_ptr<Order> OrderPtr;

std::ostream &operator<<(std::ostream &out, const Order &order);

} // namespace DistributedATS
