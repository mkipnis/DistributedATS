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

#include <map>
#include <memory>


#include <BasicDomainParticipant.h>
#include <Common.h>

#include <LogonPubSubTypes.hpp>
#include <LogoutPubSubTypes.hpp>
#include <MarketDataIncrementalRefreshPubSubTypes.hpp>
#include <MarketDataSnapshotFullRefreshPubSubTypes.hpp>
#include <NewOrderSinglePubSubTypes.hpp>
#include <ExecutionReportPubSubTypes.hpp>
#include <OrderCancelRequestPubSubTypes.hpp>
#include <OrderCancelRejectPubSubTypes.hpp>
#include <OrderMassCancelReportPubSubTypes.hpp>
#include <SecurityListPubSubTypes.hpp>
#include <OrderMassCancelRequestPubSubTypes.hpp>
#include <OrderMassStatusRequestPubSubTypes.hpp>
#include <SecurityListRequestPubSubTypes.hpp>
#include <MarketDataRequestPubSubTypes.hpp>
#include <OrderCancelReplaceRequestPubSubTypes.hpp>


namespace DistributedATS {

struct DataWriterContrainer {
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_Logon::Logon> _logon_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_NewOrderSingle::NewOrderSingle> _new_order_single_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderCancelRequest::OrderCancelRequest> _order_cancel_request_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest> _order_mass_cancel_request_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest> _order_mass_status_request_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_SecurityListRequest::SecurityListRequest> _security_list_request_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_SecurityList::SecurityList> _security_list_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_MarketDataRequest::MarketDataRequest> _market_data_request_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest> _order_cancel_replace_request_topic_tuple;
    
    distributed_ats_utils::data_writer_ptr _logon_dw;
    distributed_ats_utils::data_writer_ptr _new_order_single_dw;
    distributed_ats_utils::data_writer_ptr _order_cancel_request_dw;
    distributed_ats_utils::data_writer_ptr _order_mass_cancel_request_dw;
    distributed_ats_utils::data_writer_ptr _security_list_request_dw;
    distributed_ats_utils::data_writer_ptr _market_data_request_dw;
    distributed_ats_utils::data_writer_ptr _order_mass_status_request_dw;
    distributed_ats_utils::data_writer_ptr _order_cancel_replace_request_dw;
    
    
  DataWriterContrainer(
      distributed_ats_utils::basic_domain_participant_ptr participant_ptr) {
    //
    // Topics
    //
    _logon_topic_tuple = participant_ptr->make_topic<
      DistributedATS_Logon::LogonPubSubType,
      DistributedATS_Logon::Logon>(
        LOGON_TOPIC_NAME);
      
      _logon_dw = participant_ptr->make_data_writer(_logon_topic_tuple);
      

    _new_order_single_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_NewOrderSingle::NewOrderSinglePubSubType,
      DistributedATS_NewOrderSingle::NewOrderSingle>(
            NEW_ORDER_SINGLE_TOPIC_NAME);

      _new_order_single_dw = participant_ptr->make_data_writer(_new_order_single_topic_tuple);

     _order_cancel_request_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_OrderCancelRequest::OrderCancelRequestPubSubType,
      DistributedATS_OrderCancelRequest::OrderCancelRequest>(
            ORDER_CANCEL_REQUEST_TOPIC_NAME);
      

      _order_cancel_request_dw = participant_ptr->make_data_writer(_order_cancel_request_topic_tuple);

     _order_mass_cancel_request_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestPubSubType,
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest>(
            ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME);
      

      _order_mass_cancel_request_dw = participant_ptr->make_data_writer(_order_mass_cancel_request_topic_tuple);

      _order_cancel_replace_request_topic_tuple =
      participant_ptr->make_topic<
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestPubSubType,
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest>(
            ORDER_CANCEL_REPLACE_REQUEST_TOPIC_NAME);
      
      _order_cancel_replace_request_dw = participant_ptr->make_data_writer(_order_cancel_replace_request_topic_tuple);
      
     _order_mass_status_request_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestPubSubType,
      DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest>(
            ORDER_MASS_STATUS_REQUEST_TOPIC_NAME);

      _order_mass_status_request_dw = participant_ptr->make_data_writer(_order_mass_status_request_topic_tuple);
      

      _security_list_request_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_SecurityListRequest::SecurityListRequestPubSubType,
      DistributedATS_SecurityListRequest::SecurityListRequest>(
            SECURITY_LIST_REQUEST_TOPIC_NAME);
      
 
      _security_list_request_dw = participant_ptr->make_data_writer(_security_list_request_topic_tuple);
 
      _market_data_request_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_MarketDataRequest::MarketDataRequestPubSubType,
      DistributedATS_MarketDataRequest::MarketDataRequest>(
            MARKET_DATA_REQUEST_TOPIC_NAME);

      _market_data_request_dw = participant_ptr->make_data_writer(_market_data_request_topic_tuple);

  }
    
};

using DataWriterContrainerPtr = std::shared_ptr<DistributedATS::DataWriterContrainer>;

};
