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

#include "NewOrderSingleDataReaderListenerImpl.h"
#include "ExecutionReportDataReaderListenerImpl.h"

#include <Common.h>

#include "quickfix/FileStore.h"
#include "quickfix/NullStore.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include <BasicDomainParticipant.h>
#include <NewOrderSinglePubSubTypes.h>
#include <ExecutionReportPubSubTypes.h>

#include "LatencyTest.h"

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <thread>
#include <chrono>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>


int main( int argc, char** argv )
{
    std::string latency_test_config_file = "";
    
    boost::program_options::options_description options_desc{"Options"};
    
    options_desc.add_options()
      ("help,h", "Help screen")
      ("config,c", boost::program_options::value<std::string>()->default_value(""), "Latency Test Config File");
    
    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, options_desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
      std::cout << options_desc << '\n';
    else if (vm.count("config"))
        latency_test_config_file = vm["config"].as<std::string>();

    if (latency_test_config_file.empty()) {
            std::cerr << "Error: Config file name is not specified." << std::endl;
            return -1;
   }


  FIX::Initiator * initiator = 0;

  try
  {

        auto settings = std::make_shared<FIX::SessionSettings>(latency_test_config_file);
      
        //FIX::SessionID default_session_id("FIX.4.4", "DEFAULT", "DEFAULT");
        
        //auto session_settings = settings->get(default_session_id);
      
        FIX::SessionID default_session_id = *settings->getSessions().begin();

        std::cout << "Default Sesison ID : " << default_session_id << std::endl;
      
        auto dats_home = std::getenv("DATS_HOME");
        auto dats_log_home = std::getenv("DATS_LOG_HOME");
      
        if ( dats_home == NULL || dats_log_home == NULL )
            throw std::runtime_error("DATS_HOME or/and DATS_LOG_HOME is not set");
      
        auto default_dictionary_tmp = &settings->get(default_session_id);
        const_cast<FIX::Dictionary *>(default_dictionary_tmp)->setString("DATADICTIONARY", std::string(dats_home) + "/spec/" + default_dictionary_tmp->getString("DATADICTIONARY") );


     LatencyTest::LatencyStatsPtr latencyStatsPtr( std::make_shared<LatencyTest::latency_stats>() );

     auto participant_ptr =
          std::make_shared<distributed_ats_utils::basic_domain_participant>(0, "LatencyTest");

      participant_ptr->create_subscriber();
      participant_ptr->create_publisher();


    auto new_order_single_topic_topic = participant_ptr->make_topic<
      DistributedATS_NewOrderSingle::NewOrderSinglePubSubType,
      DistributedATS_NewOrderSingle::NewOrderSingle>(
            NEW_ORDER_SINGLE_TOPIC_NAME);
      
      auto new_order_single_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(new_order_single_topic_topic,
                                                new LatencyTest::NewOrderSingleDataReaderListenerImpl( latencyStatsPtr ));
      
      auto execution_report_topic_tuple = participant_ptr->make_topic<
    DistributedATS_ExecutionReport::ExecutionReportPubSubType,
    DistributedATS_ExecutionReport::ExecutionReport>(
          EXECUTION_REPORT_TOPIC_NAME);
    
    auto execution_report_data_reader_tuple = participant_ptr->make_data_reader_tuple( execution_report_topic_tuple,
                                        new LatencyTest::ExecutionReportDataReaderListenerImpl( latencyStatsPtr  ));


    LatencyTest::Application application( *settings, latencyStatsPtr );
    FIX::NullStoreFactory storeFactory;
    FIX::ScreenLogFactory logFactory( *settings );

      try
      {
          initiator = new FIX::SocketInitiator( application, storeFactory, *settings, logFactory );
          
          initiator->start();
          application.run();
          initiator->stop();
          delete initiator;
      }
      catch ( std::exception & e )
      {
          std::cout << e.what();
          delete initiator;
          return 1;
      }

    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
    delete initiator;
    return 1;
  }
}
