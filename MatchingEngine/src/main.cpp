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

#include <BasicDomainParticipant.h>
#include <ExecutionReportTypeSupportImpl.h>
#include <MarketDataRequestTypeSupportImpl.h>
#include <NewOrderSingleTypeSupportImpl.h>
#include <OrderCancelRejectTypeSupportImpl.h>
#include <OrderCancelRequestTypeSupportImpl.h>
#include <OrderMassCancelReportTypeSupportImpl.h>
#include <OrderMassCancelRequestTypeSupportImpl.h>
#include <SecurityListRequestTypeSupportImpl.h>
#include <SecurityListTypeSupportImpl.h>
#include <OrderCancelReplaceRequestTypeSupportImpl.h>

#include <book/depth_order_book.h>

#include <Common.h>

#include <ConfigFileHelper.h>

#include <ace/Get_Opt.h>

#include <ace/Sig_Adapter.h>
#include <ace/Signal.h>

#include "ExecutionReportTypeSupportImpl.h"
#include "MarketDataIncrementalRefreshTypeSupportImpl.h"
#include "MarketDataSnapshotFullRefreshDataReaderListenerImpl.h"
#include "NewOrderSingleDataReaderListenerImpl.h"
#include "OrderCancelRequestDataReaderListenerImpl.h"
#include "OrderMassCancelRequestDataReaderListenerImpl.h"
#include "SecurityListDataReaderListenerImpl.h"
#include "OrderCancelReplaceRequestDataReaderListenerImpl.h"

#include "Market.h"

#include <time.h>

#include <ace/Message_Queue_T.h>

#include <MarketDataIncrementalRefreshLogger.hpp>

#include <LoggerHelper.h>

#include "PriceDepthPublisherService.hpp"

#include <atomic>

std::atomic<bool> is_running;

namespace DistributedATS {

class SignalHandler : public ACE_Event_Handler {
public:
  virtual int handle_signal(int signum, siginfo_t * = 0, ucontext_t * = 0) {
    std::cout << "Lets hangle signal : " << signum << std::endl;
    is_running.store(false);
    return 0;
  };
};

}; // namespace DistributedATS

int main(int argc, char *argv[]) {
  DDS::DomainParticipantFactory_var dpf = DDS::DomainParticipantFactory::_nil();

  try {
    dpf = TheParticipantFactoryWithArgs(argc, argv);

    if (argc < 2) {
      std::cout << "usage: " << argv[0] << std::endl
                << "\t\t-c matching_engine_config_file_name" << std::endl;
      return -1;
    }

    ACE_Get_Opt cmd_opts(argc, argv, ":c:");

    int option;
    // std::string market_name = "";
    // std::string data_service_name = "";

    std::string matching_engine_config_file = "";

    while ((option = cmd_opts()) != EOF) {
      switch (option) {
      case 'c':
        matching_engine_config_file = cmd_opts.opt_arg();
        break;
      }
    }

    if (matching_engine_config_file.empty()) {
      std::cerr << "Error: Config file name is not specified." << std::endl;
      return -1;
    }

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) INFO: Config File : [%s]\n"),
               matching_engine_config_file.c_str()));

    ConfigFileHelper configFileHelper(matching_engine_config_file.c_str());

    ACE_TString market_name;
    if (configFileHelper.get_string_value(
            ACE_TEXT("matchingengine"), ACE_TEXT("market_name"), market_name)) {
      ACE_ERROR(
          (LM_ERROR,
           ACE_TEXT(
               "(%P|%t) ERROR: Unable to get market name from ini file %d.\n"),
           ACE_OS::last_error()));
    };

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) INFO: MarketName:[%s]\n"),
               market_name.c_str()));

    ACE_TString data_service_name;
    if (configFileHelper.get_string_value(ACE_TEXT("matchingengine"),
                                          ACE_TEXT("dataservice_name"),
                                          data_service_name)) {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT("(%P|%t) ERROR: Unable to get data service name from "
                          "ini file %d.\n"),
                 ACE_OS::last_error()));
    };
      

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) INFO: DataService:[%s]\n"),
               data_service_name.c_str()));

    if (market_name.empty() || data_service_name.empty()) {
      std::cerr << "No market name specified or no data service association in "
                   "config file"
                << std::endl;
      return -1;
    }
      
    int price_depth_pub_interval;
    if (configFileHelper.get_integer_value(ACE_TEXT("marketdata"),
                                            ACE_TEXT("price_depth_pub_interval"),
                                           &price_depth_pub_interval)) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: Unable to get price depth publication interval from "
                              "ini file %d.\n"),
                     ACE_OS::last_error()));
    };

    std::cout << "Market Name : " << market_name
              << " Data Service Name : " << data_service_name << " Price Depth Publication Interval :" <<  price_depth_pub_interval << std::endl;

    ACE_Sig_Handler handler;
    DistributedATS::SignalHandler signalHandler;

    // add signal handler for the SIGINT signal here
    ACE_Sig_Handler sig_handler;
    sig_handler.register_handler(SIGINT, &signalHandler);

    distributed_ats_utils::BasicDomainParticipant participant(
        dpf, DISTRIBUTED_ATS_DOMAIN_ID);
    participant.createSubscriber();
    participant.createPublisher();

    // Enqueue incremental market data update,
    // but publish conflated updates from
    // the MarketData Service Thread
    auto price_depth_publisher_queue_ptr =
            std::make_shared<DistributedATS::PriceDepthPublisherQueue>();

    auto market =
        std::make_shared<DistributedATS::Market>(
            market_name.c_str(), data_service_name.c_str(),
            price_depth_publisher_queue_ptr);

    std::atomic_init(&is_running, true);

    // Filter expression for the MARKET_NAME specified in config file
    // filter for messages for this market/security exchange
    std::string filter_str =
        "m_Header.TargetCompID = 'MATCHING_ENGINE' and SecurityExchange='" +
        market->getMarketName() + "'";
    // filter for mass cancel
    std::string filter_str_target_only =
        "m_Header.TargetCompID = 'MATCHING_ENGINE'";

    // Incoming data
    // New Order Single:
    // Topic
    DDS::Topic_var new_order_single_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_NewOrderSingle::NewOrderSingleTypeSupport_var,
      DistributedATS_NewOrderSingle::NewOrderSingleTypeSupportImpl>(
            NEW_ORDER_SINGLE_TOPIC_NAME);
    // Filter
    DDS::ContentFilteredTopic_ptr cft =
        participant.getDomainParticipant()->create_contentfilteredtopic(
            "FILTER_MATCHING_ENGINE_NEW_ORDER_SINGLE", new_order_single_topic,
            filter_str.c_str(), DDS::StringSeq());
    // Data Reader
    DDS::DataReaderListener_var newOrderSingleDataListener(
        new MatchingEngine::NewOrderSingleDataReaderListenerImpl(market));
    participant.createDataReaderListener(cft, newOrderSingleDataListener);

    // Order cancel
    // Topic
    DDS::Topic_var order_cancel_request_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_OrderCancelRequest::OrderCancelRequestTypeSupport_var,
      DistributedATS_OrderCancelRequest::OrderCancelRequestTypeSupportImpl>(
            ORDER_CANCEL_REQUEST_TOPIC_NAME);
    // Filter
    DDS::ContentFilteredTopic_ptr cft_cancel =
        participant.getDomainParticipant()->create_contentfilteredtopic(
            "FILTER_MATCHING_ENGINE_ORDER_CANCEL_REQUEST",
            order_cancel_request_topic, filter_str.c_str(), DDS::StringSeq());
    // Data Reader
    DDS::DataReaderListener_var orderCancelRequestDataListener(
        new MatchingEngine::OrderCancelRequestDataReaderListenerImpl(market));
    participant.createDataReaderListener(cft_cancel,
                                         orderCancelRequestDataListener);
   
    // Mass cancel requests
    // Topic
    DDS::Topic_var order_mass_cancel_request_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestTypeSupport_var,
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestTypeSupportImpl>(
            ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME);
    // Filter
    DDS::ContentFilteredTopic_ptr cft_mass_cancel =
        participant.getDomainParticipant()->create_contentfilteredtopic(
            "FILTER_MATCHING_ENGINE_ORDER_MASS_CANCEL_REQUEST",
            order_mass_cancel_request_topic, filter_str_target_only.c_str(),
            DDS::StringSeq());
    // Data Reader
    DDS::DataReaderListener_var orderMassCancelRequestDataListener(
        new MatchingEngine::OrderMassCancelRequestDataReaderListenerImpl(
            market));
    participant.createDataReaderListener(cft_mass_cancel,
                                         orderMassCancelRequestDataListener);
      
      // Order cancel/replace requests
      // Topic
      DDS::Topic_var order_cancel_replace_request_topic =
          participant.createTopicAndRegisterType<
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestTypeSupport_var,
        DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestTypeSupportImpl>(
              ORDER_CANCEL_REPLACE_REQUEST_TOPIC_NAME);
      // Filter
      DDS::ContentFilteredTopic_ptr cft_cancel_replace =
          participant.getDomainParticipant()->create_contentfilteredtopic(
              "FILTER_MATCHING_ENGINE_ORDER_CANCEL_REPLACE_REQUEST",
              order_cancel_replace_request_topic, filter_str_target_only.c_str(),
              DDS::StringSeq());
      // Data Reader
      DDS::DataReaderListener_var orderCancelReplaceRequestDataListener(
          new MatchingEngine::OrderCancelReplaceRequestDataReaderListenerImpl(
              market));
      participant.createDataReaderListener(cft_cancel_replace,
                                           orderCancelReplaceRequestDataListener);
   
    // Topic to receive list of securities to setup order books
    // Topic
    DDS::Topic_var security_list_topic = participant.createTopicAndRegisterType<
      DistributedATS_SecurityList::SecurityListTypeSupport_var,
      DistributedATS_SecurityList::SecurityListTypeSupportImpl>(
        SECURITY_LIST_TOPIC_NAME);
    // Filter
    DDS::ContentFilteredTopic_ptr cft_security_list =
        participant.getDomainParticipant()->create_contentfilteredtopic(
            "FILTER_MATCHING_ENGINE_SECURITY_LIST", security_list_topic,
            std::string("m_Header.TargetCompID = '" + market->getMarketName() +
                        "'")
                .c_str(),
            DDS::StringSeq());
    // Data Reader
    DDS::DataReaderListener_var securityListDataListener(
        new DistributedATS::SecurityListDataReaderListenerImpl(market));
    participant.createDataReaderListener(cft_security_list,
                                         securityListDataListener);
   
    // Topic to receive market open prices
    // Topic
    DDS::Topic_var market_data_snapshot_full_refresh_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshTypeSupport_var,
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshTypeSupportImpl>(
            MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME);
    // Filter
    DDS::ContentFilteredTopic_ptr cft_market_data_full_snapshot =
        participant.getDomainParticipant()->create_contentfilteredtopic(
            "FILTER_MATCHING_ENGINE_FULL_SNAPSHOT_REQUEST",
            market_data_snapshot_full_refresh_topic,
            std::string("m_Header.TargetCompID = '" + market->getMarketName() +
                        "'")
                .c_str(),
            DDS::StringSeq());
    // Data Reader
    DDS::DataReaderListener_var marketDataFullSnapshotDataListener(
        new DistributedATS::MarketDataSnapshotFullRefreshDataReaderListenerImpl(
            market));
    participant.createDataReaderListener(cft_market_data_full_snapshot,
                                         marketDataFullSnapshotDataListener);
    //
    // Outgoing data
    //
    // Execution Reports
    // Topic
    DDS::Topic_var execution_report_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_ExecutionReport::ExecutionReportTypeSupport_var,
      DistributedATS_ExecutionReport::ExecutionReportTypeSupportImpl>(
            EXECUTION_REPORT_TOPIC_NAME);
    // Data Writer
      DistributedATS_ExecutionReport::ExecutionReportDataWriter_var execution_report_dw =
        participant.createDataWriter<
      DistributedATS_ExecutionReport::ExecutionReportDataWriter_var,
      DistributedATS_ExecutionReport::ExecutionReportDataWriter>(
            execution_report_topic);
    // Market Setter
    market->setExecutionReportDataWriter(execution_report_dw);

    // Topic to publish status of mass cancel report
    // Topic
    DDS::Topic_var order_mass_cancel_report_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportTypeSupport_var,
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportTypeSupportImpl>(
            ORDER_MASS_CANCEL_REPORT_TOPIC_NAME);
    // Data Writer
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataWriter_var
        order_mass_cancel_report_dw = participant.createDataWriter<
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataWriter_var,
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataWriter>(
            order_mass_cancel_report_topic);
    // Market Setter
    market->setOrderMassCancelReportDataWriter(order_mass_cancel_report_dw);

    // Topic to request list of securities to setup order books
    // Topic
    DDS::Topic_var security_list_request_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_SecurityListRequest::SecurityListRequestTypeSupport_var,
      DistributedATS_SecurityListRequest::SecurityListRequestTypeSupportImpl>(
            SECURITY_LIST_REQUEST_TOPIC_NAME);
    // Data Writer
      DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_var
        security_list_request_dw = participant.createDataWriter<
      DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_var,
      DistributedATS_SecurityListRequest::SecurityListRequestDataWriter>(
            security_list_request_topic);
    // Market Setter
    market->setSecurityListRequestDataWriter(security_list_request_dw);

    // Topic to publish order cancel reject
    // Topic
    DDS::Topic_var order_cancel_reject_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_OrderCancelReject::OrderCancelRejectTypeSupport_var,
      DistributedATS_OrderCancelReject::OrderCancelRejectTypeSupportImpl>(
            ORDER_CANCEL_REJECT_TOPIC_NAME);
    // Data Writer
      DistributedATS_OrderCancelReject::OrderCancelRejectDataWriter_var
        order_cancel_reject_dw = participant.createDataWriter<
      DistributedATS_OrderCancelReject::OrderCancelRejectDataWriter_var,
      DistributedATS_OrderCancelReject::OrderCancelRejectDataWriter>(
            order_cancel_reject_topic);
    // Market Setter
    market->setOrderCancelRejectDataWriter(order_cancel_reject_dw);

    // Topic to publish conflated incremental market data refresh
    // Topic
    DDS::Topic_var market_data_incremental_refresh_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshTypeSupport_var,
      DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshTypeSupportImpl>(
            MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);
    // Data Writer
      DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshDataWriter_var
        market_data_incremental_refresh_dw = participant.createDataWriter<
      DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshDataWriter_var,
      DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshDataWriter>(
            market_data_incremental_refresh_topic);
    // Market Setter
    market->setMarketDataIncrementalRefreshDataWriter(
        market_data_incremental_refresh_dw);

    // Market Data Request
    // Topic
    DDS::Topic_var market_data_request_topic =
        participant.createTopicAndRegisterType<
      DistributedATS_MarketDataRequest::MarketDataRequestTypeSupport_var,
      DistributedATS_MarketDataRequest::MarketDataRequestTypeSupportImpl>(
            MARKET_DATA_REQUEST_TOPIC_NAME);
    // Data Writer
      DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_var
        marketDataRequestDataWriter = participant.createDataWriter<
      DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_var,
      DistributedATS_MarketDataRequest::MarketDataRequestDataWriter>(
            market_data_request_topic);
    // Market Setter
    market->setMarketDataRequestDataWriter(marketDataRequestDataWriter);

    //
    // We should be all set with topics, data readers and data writers setup.
    // Lets publish security list request to a DataService.
    //
    // SecurityListDataReader should recieve set of instruments from DataService
    // and setup order books in LiquiBook.
    market->publishSecurityListRequest();

    // lets create and active conflated market data publisher
    auto price_depth_publisher_service_ptr =
        std::make_shared<DistributedATS::PriceDepthPublisherService>(
            market_data_incremental_refresh_dw,
            price_depth_publisher_queue_ptr,
            price_depth_pub_interval);

    price_depth_publisher_service_ptr->activate();

    while (is_running == true)
      ACE_OS::sleep(1);

    participant.getDomainParticipant()->delete_subscriber(
        participant.getSubscriber());

    market->cancel_all_orders();

  } catch (std::exception &e) {
      
      ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Exception during the initialization of Matching Engine : %s\n"), e.what()));

      
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
};
