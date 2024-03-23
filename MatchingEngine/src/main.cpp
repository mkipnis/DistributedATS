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

#include <BasicDomainParticipant.h>
#include <book/depth_order_book.h>

#include <Common.h>

#include "Market.h"

#include <time.h>
#include <MarketDataIncrementalRefreshLogger.hpp>

#include <LoggerHelper.h>

#include "DataWriterContainer.h"
#include "PriceDepthPublisherService.hpp"

#include <MarketDataSnapshotFullRefreshPubSubTypes.h>
#include <NewOrderSinglePubSubTypes.h>
#include <ExecutionReportPubSubTypes.h>
#include <OrderCancelRejectPubSubTypes.h>
#include <OrderMassCancelReportPubSubTypes.h>
#include <OrderCancelRequestPubSubTypes.h>
#include <SecurityListPubSubTypes.h>
#include <OrderMassCancelRequestPubSubTypes.h>
#include <OrderCancelReplaceRequestPubSubTypes.h>
#include <SecurityListRequestPubSubTypes.h>
#include <MarketDataIncrementalRefreshPubSubTypes.h>
#include <MarketDataRequestPubSubTypes.h>

#include "NewOrderSingleDataReaderListenerImpl.h"
#include "OrderCancelRequestDataReaderListenerImpl.h"
#include "OrderMassCancelRequestDataReaderListenerImpl.h"
#include "OrderCancelReplaceRequestDataReaderListenerImpl.h"
#include "SecurityListDataReaderListenerImpl.h"
#include "MarketDataSnapshotFullRefreshDataReaderListenerImpl.h"
#include "SecurityListRequestDataWriterListener.h"

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <thread>
#include <chrono>


#include <atomic>

std::atomic<bool> is_running;


int main(int argc, char *argv[]) {

    
  try {

      std::string matching_engine_config_file = "";
      
      boost::program_options::options_description options_desc{"Options"};
      
      options_desc.add_options()
        ("help,h", "Help screen")
        ("config,c", boost::program_options::value<std::string>()->default_value(""), "Matching Engine Config File");
      
      boost::program_options::variables_map vm;
      boost::program_options::store(parse_command_line(argc, argv, options_desc), vm);
      boost::program_options::notify(vm);

      if (vm.count("help"))
          std::cout << options_desc << '\n';
      else if (vm.count("config"))
          matching_engine_config_file = vm["config"].as<std::string>();
      
      
      if ( matching_engine_config_file.empty() )
      {
          std::cerr << "Error: Config file name is not specified." << std::endl;
          return -1;
      }
          
      boost::property_tree::ptree pt;
      boost::property_tree::ini_parser::read_ini(matching_engine_config_file, pt);
          
      std::string data_service_name = pt.get<std::string>("matchingengine.dataservice_name");
      std::string market_name = pt.get<std::string>("matchingengine.market_name");

      
      int price_depth_pub_interval = pt.get<int>("marketdata.price_depth_pub_interval");

      LOG4CXX_INFO(logger, "Market Name|" << market_name
                   << "|Data Service Name|" << data_service_name << "|Price Depth Publication Interval|" <<  price_depth_pub_interval );

      // Enqueue incremental market data update,
      // but publish conflated updates from
      // the MarketData Service Thread
      auto price_depth_publisher_queue_ptr =
            std::make_shared<DistributedATS::PriceDepthPublisherQueue>();
        
      auto dataWriterContainer = std::make_shared<DistributedATS::DataWriterContainer>();

      auto market =
        std::make_shared<DistributedATS::Market>(dataWriterContainer,
            market_name, data_service_name,
            price_depth_publisher_queue_ptr);

      std::atomic_init(&is_running, true);

      // Filter expression for the MARKET_NAME specified in config file
      // filter for messages for this market/security exchange
      std::string filter_str =
            "header.TargetCompID = 'MATCHING_ENGINE' and SecurityExchange='" +
            market->getMarketName() + "'";
      // filter for mass cancel
      std::string filter_str_target_only =
            "m_Header.TargetCompID = 'MATCHING_ENGINE'";
      
      auto participant_ptr =
          std::make_shared<distributed_ats_utils::basic_domain_participant>(0, "MatchingEngine");

      participant_ptr->create_subscriber();
      participant_ptr->create_publisher();

      // Incoming data
      // New Order Single:
      // Topic
      auto  new_order_single_topic_tuple =
        participant_ptr->make_topic<
            DistributedATS_NewOrderSingle::NewOrderSinglePubSubType,
            DistributedATS_NewOrderSingle::NewOrderSingle>(NEW_ORDER_SINGLE_TOPIC_NAME);
      
      auto new_order_single_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(new_order_single_topic_tuple,
                            new MatchingEngine::NewOrderSingleDataReaderListenerImpl(market),
                            "FILTER_MATCHING_ENGINE_NEW_ORDER_SINGLE", filter_str);
    
      
      auto order_cancel_request_topic_tuple = participant_ptr->make_topic<
        DistributedATS_OrderCancelRequest::OrderCancelRequestPubSubType,
        DistributedATS_OrderCancelRequest::OrderCancelRequest>(ORDER_CANCEL_REQUEST_TOPIC_NAME);
      
      auto order_cancel_request_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(order_cancel_request_topic_tuple,
                              new MatchingEngine::OrderCancelRequestDataReaderListenerImpl(market)/*,
                              "FILTER_MATCHING_ENGINE_ORDER_CANCEL_REQUEST", filter_str*/);
   
      
      auto order_mass_cancel_request_topic_tuple = participant_ptr->make_topic<
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestPubSubType,
      DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest>(ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME);
      auto order_mass_cancel_request_topic_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(order_mass_cancel_request_topic_tuple,
                              new MatchingEngine::OrderMassCancelRequestDataReaderListenerImpl(market)/*,
                              "FILTER_MATCHING_ENGINE_ORDER_MASS_CANCEL_REQUEST", filter_str*/);
      
      auto order_cancel_replace_request_topic_tuple = participant_ptr->make_topic<
      DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestPubSubType,
      DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest>(ORDER_CANCEL_REPLACE_REQUEST_TOPIC_NAME);
      auto order_cancel_replace_request_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(order_cancel_replace_request_topic_tuple,
                              new MatchingEngine::OrderCancelReplaceRequestDataReaderListenerImpl(market)/*,
                              "FILTER_MATCHING_ENGINE_ORDER_CANCEL_REPLACE_REQUEST", filter_str*/);
  
      auto security_list_topic_tuple = participant_ptr->make_topic<
      DistributedATS_SecurityList::SecurityListPubSubType,
      DistributedATS_SecurityList::SecurityList>(SECURITY_LIST_TOPIC_NAME);
      auto security_list_topic_request_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(security_list_topic_tuple,
                              new DistributedATS::SecurityListDataReaderListenerImpl(market)/*,
                              "FILTER_MATCHING_ENGINE_SECURITY_LIST", std::string("header.TargetCompID = '" + market->getMarketName() + "'")*/);
      
      auto market_data_snapshot_full_refresh_topic_tuple = participant_ptr->make_topic<
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshPubSubType,
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefresh>(MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME);
      auto market_data_snapshot_full_refresh_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(market_data_snapshot_full_refresh_topic_tuple,
                              new DistributedATS::MarketDataSnapshotFullRefreshDataReaderListenerImpl(market)/*,
                              "FILTER_MATCHING_ENGINE_FULL_SNAPSHOT_REQUEST", std::string("header.TargetCompID = '" + market->getMarketName() + "'")*/);
      
      //
      // Outgoing data
      //
      // Execution Reports
      // Topic
      auto execution_report_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_ExecutionReport::ExecutionReportPubSubType,
      DistributedATS_ExecutionReport::ExecutionReport>(
            EXECUTION_REPORT_TOPIC_NAME);;
      // Data Writer
      dataWriterContainer->execution_report_dw =
      participant_ptr->make_data_writer(execution_report_topic_tuple);

    // Topic to publish status of mass cancel report
    // Topic
    auto order_mass_cancel_report_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportPubSubType,
      DistributedATS_OrderMassCancelReport::OrderMassCancelReport>(
            ORDER_MASS_CANCEL_REPORT_TOPIC_NAME);

      dataWriterContainer->order_mass_cancel_report_dw = participant_ptr->make_data_writer(
            order_mass_cancel_report_topic_tuple);

      // Topic to request list of securities to setup order books
      // Topic
      auto security_list_request_topic_tuple =
        participant_ptr->make_topic<
        DistributedATS_SecurityListRequest::SecurityListRequestPubSubType,
        DistributedATS_SecurityListRequest::SecurityListRequest>(
            SECURITY_LIST_REQUEST_TOPIC_NAME);
      
        DistributedATS::security_list_request_data_writer_listener_ptr
        sec_list_req_dw_ptr(new DistributedATS::SecurityListRequestDataWriterListener(market));

        dataWriterContainer->security_list_request_dw =
                participant_ptr->make_data_writer(security_list_request_topic_tuple, sec_list_req_dw_ptr.get());


      // Topic to publish order cancel reject
      // Topic
      auto order_cancel_reject_topic_tuple =
        participant_ptr->make_topic<
        DistributedATS_OrderCancelReject::OrderCancelRejectPubSubType,
        DistributedATS_OrderCancelReject::OrderCancelReject>(
            ORDER_CANCEL_REJECT_TOPIC_NAME);

        dataWriterContainer->order_cancel_reject_dw =
            participant_ptr->make_data_writer(order_cancel_reject_topic_tuple);


      // Topic to publish conflated incremental market data refresh
      // Topic
      auto market_data_incremental_refresh_topic_tuple =
        participant_ptr->make_topic<
        DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshPubSubType,
        DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefresh>(
            MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);

      dataWriterContainer->market_data_incremental_refresh_dw = participant_ptr->make_data_writer(
                                                                               market_data_incremental_refresh_topic_tuple);
    // Market Setter
     /// dataWriterContainer->setMarketDataIncrementalRefreshDataWriter(
      //  market_data_incremental_refresh_dw);

    // Market Data Request
    // Topic
    auto market_data_request_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_MarketDataRequest::MarketDataRequestPubSubType,
      DistributedATS_MarketDataRequest::MarketDataRequest>(
            MARKET_DATA_REQUEST_TOPIC_NAME);
    // Data Writer
      dataWriterContainer->market_data_request_dw =
        participant_ptr->make_data_writer(market_data_request_topic_tuple);


    // lets create and active conflated market data publisher
    auto price_depth_publisher_service_ptr =
        std::make_shared<DistributedATS::PriceDepthPublisherService>(
            dataWriterContainer->market_data_incremental_refresh_dw.get(),
            price_depth_publisher_queue_ptr,
            price_depth_pub_interval);
      
      std::atomic_init(&is_running, true);

      boost::asio::io_service io_service;
      boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
      
      signals.async_wait([&](const boost::system::error_code& ec, int signal_number) {
          if (!ec) {
              std::cout << "Signal number " << signal_number << std::endl;
              std::cout << "Gracefully stopping the timer and exiting"
                        << std::endl;
              is_running.store(false);
          } else {
              std::cout << "Error " << ec.value() << " - " << ec.message()
                        << " - Signal number - " << signal_number << std::endl;
          }
      });
      
      io_service.run();

   
  } catch (std::exception &e) {
      
      
      LOG4CXX_ERROR(logger, "Exception during the initialization of Matching Engine :" << e.what());
    
      
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
};
