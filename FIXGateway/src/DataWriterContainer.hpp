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

#ifndef DataWriterSingleton_hpp
#define DataWriterSingleton_hpp

#include <ace/Singleton.h>

#include <ace/Guard_T.h>
#include <ace/RW_Mutex.h>
#include <map>
#include <memory>

#include <LogonTypeSupportImpl.h>
#include <MarketDataRequestTypeSupportImpl.h>
#include <NewOrderSingleTypeSupportImpl.h>
#include <OrderCancelRequestTypeSupportImpl.h>
#include <OrderMassCancelRequestTypeSupportImpl.h>
#include <OrderMassStatusRequestAdapter.hpp>
#include <SecurityListRequestTypeSupportImpl.h>
#include <OrderCancelReplaceRequestTypeSupportImpl.h>
#include <dds/DdsDcpsPublicationC.h>

#include <BasicDomainParticipant.h>
#include <Common.h>

namespace DistributedATS {

struct DataWriterContrainer {
  DataWriterContrainer(
      distributed_ats_utils::BasicDomainParticipantPtr participantPtr) {
    //
    // Topics
    //
    DDS::Topic_var logon_topic = participantPtr->createTopicAndRegisterType<
      DistributedATS_Logon::LogonTypeSupport_var, DistributedATS_Logon::LogonTypeSupportImpl>(
        LOGON_TOPIC_NAME);

    DDS::Topic_var new_order_single_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_NewOrderSingle::NewOrderSingleTypeSupport_var,
            DistributedATS_NewOrderSingle::NewOrderSingleTypeSupportImpl>(
            NEW_ORDER_SINGLE_TOPIC_NAME);

    DDS::Topic_var order_cancel_request_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_OrderCancelRequest::OrderCancelRequestTypeSupport_var,
      DistributedATS_OrderCancelRequest::OrderCancelRequestTypeSupportImpl>(
            ORDER_CANCEL_REQUEST_TOPIC_NAME);

    DDS::Topic_var order_mass_cancel_request_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestTypeSupport_var,
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestTypeSupportImpl>(
            ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME);

    DDS::Topic_var order_mass_status_request_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestTypeSupport_var,
      DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestTypeSupportImpl>(
            ORDER_MASS_STATUS_REQUEST_TOPIC_NAME);

    DDS::Topic_var security_list_request_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_SecurityListRequest::SecurityListRequestTypeSupport_var,
      DistributedATS_SecurityListRequest::SecurityListRequestTypeSupportImpl>(
            SECURITY_LIST_REQUEST_TOPIC_NAME);

    DDS::Topic_var market_data_request_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_MarketDataRequest::MarketDataRequestTypeSupport_var,
      DistributedATS_MarketDataRequest::MarketDataRequestTypeSupportImpl>(
            MARKET_DATA_REQUEST_TOPIC_NAME);
      
      DDS::Topic_var order_cancel_replace_request_topic =
          participantPtr->createTopicAndRegisterType<
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestTypeSupport_var,
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestTypeSupportImpl>(
            ORDER_CANCEL_REPLACE_REQUEST_TOPIC_NAME);

    //
    // Data Writers
    //
    _logonDataWriter =
        participantPtr->createDataWriter<DistributedATS_Logon::LogonDataWriter_var,
      DistributedATS_Logon::LogonDataWriter>(
            logon_topic);

    _newOrderSingleDataWriter =
        participantPtr
            ->createDataWriter<DistributedATS_NewOrderSingle::NewOrderSingleDataWriter_var,
      DistributedATS_NewOrderSingle::NewOrderSingleDataWriter>(
                new_order_single_topic);

    _orderCancelRequestDataWriter = participantPtr->createDataWriter<
      DistributedATS_OrderCancelRequest::OrderCancelRequestDataWriter_var,
      DistributedATS_OrderCancelRequest::OrderCancelRequestDataWriter>(
        order_cancel_request_topic);

    _orderMassCancelRequestDataWriter = participantPtr->createDataWriter<
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestDataWriter_var,
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestDataWriter>(
        order_mass_cancel_request_topic);

    _orderMassStatusRequestDataWriter = participantPtr->createDataWriter<
      DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestDataWriter_var,
      DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestDataWriter>(
        order_mass_status_request_topic);

    _securityListRequestDataWriter = participantPtr->createDataWriter<
      DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_var,
      DistributedATS_SecurityListRequest::SecurityListRequestDataWriter>(
        security_list_request_topic);

    _marketDataRequestDataWriter = participantPtr->createDataWriter<
      DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_var,
      DistributedATS_MarketDataRequest::MarketDataRequestDataWriter>(
        market_data_request_topic);
      
    _orderCancelReplaceRequestDataWriter = participantPtr->createDataWriter<
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestDataWriter_var,
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestDataWriter>(order_cancel_replace_request_topic);
  }

    DistributedATS_Logon::LogonDataWriter_var _logonDataWriter;
    DistributedATS_NewOrderSingle::NewOrderSingleDataWriter_var _newOrderSingleDataWriter;
    DistributedATS_OrderCancelRequest::OrderCancelRequestDataWriter_ptr
      _orderCancelRequestDataWriter;
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestDataWriter_ptr
      _orderMassCancelRequestDataWriter;
    DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_ptr
      _securityListRequestDataWriter;
    DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_ptr
      _marketDataRequestDataWriter;
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestDataWriter_ptr
      _orderMassStatusRequestDataWriter;
    
    DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestDataWriter_ptr
      _orderCancelReplaceRequestDataWriter;
};

}; // namespace DistributedATS

#endif /* DataWriterSingleton_hpp */
