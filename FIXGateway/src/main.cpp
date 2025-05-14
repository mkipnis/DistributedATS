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

#include <iostream>

#include <LogonAdapter.hpp>
#include <LogoutAdapter.hpp>

#include <NewOrderSingleAdapter.hpp>
#include <OrderCancelRejectAdapter.hpp>
#include <OrderCancelRequestAdapter.hpp>
#include <OrderMassCancelReportAdapter.hpp>
#include <OrderMassCancelRequestAdapter.hpp>

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

#include <BasicDomainParticipant.h>

#include "quickfix/FileStore.h"
#include "quickfix/Log.h"
#include "quickfix/NullStore.h"
#include "quickfix/SessionSettings.h"
#include <fstream>
#include <iostream>
#include <quickfix/SessionFactory.h>
#include <string>

#include "SocketAcceptor.h"
#include "SocketConnection.h"

#include <BasicDomainParticipant.h>
#include <Common.h>

#include "ExecutionReportDataReaderListenerImpl.h"
#include "LogonDataReaderListenerImpl.hpp"
#include "LogoutDataReaderListenerImpl.hpp"
#include "OrderCancelRejectDataReaderListenerImpl.h"
#include "OrderMassCancelReportDataReaderListenerImpl.h"
#include "SecurityListDataReaderListenerImpl.h"

#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"
#include "MarketDataSnapshotFullRefreshDataReaderListenerImpl.h"

#include "DataReaderContainer.h"
#include "DataWriterContainer.hpp"

#include "Application.hpp"
#include "AuthServiceHelper.h"

#include <fstream>

#include "FileLog.h"

#include <atomic>
#include <log4cxx/logger.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/mdc.h>
#include <log4cxx/xml/domconfigurator.h>


std::atomic<bool> is_running;


int main(int argc, char **argv)
{
    std::string quickfix_config_file = "";
    std::string sender_comp_id = "";
    std::string data_service_name = "";
    
    LOG4CXX_INFO(logger, "Starting up");

    try {

        boost::program_options::options_description options_desc{"Options"};
      
        options_desc.add_options()
            ("help,h", "Help screen")
            ("config,c", boost::program_options::value<std::string>()->default_value(""), "QuickFIX config file");
      
        boost::program_options::variables_map vm;
        boost::program_options::store(parse_command_line(argc, argv, options_desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
            std::cout << options_desc << '\n';
        else if (vm.count("config"))
            quickfix_config_file = vm["config"].as<std::string>();

        is_running = true;
        
        auto settings = std::make_shared<FIX::SessionSettings>(quickfix_config_file);
      
        FIX::SessionID default_session_id("FIX.4.4", "DEFAULT", "DEFAULT");
        
        auto session_settings = settings->get(default_session_id);
          
        auto dats_home = std::getenv("DATS_HOME");
        auto dats_log_home = std::getenv("DATS_LOG_HOME");
      
        if ( dats_home == NULL || dats_log_home == NULL )
            throw std::runtime_error("DATS_HOME or/and DATS_LOG_HOME is not set");

        auto default_dictionary_tmp = &settings->get(default_session_id);
        const_cast<FIX::Dictionary *>(default_dictionary_tmp)->setString("DATADICTIONARY", std::string(dats_home) + "/spec/" + default_dictionary_tmp->getString("DATADICTIONARY") );
        
        auto default_dictionary = std::make_shared<FIX::Dictionary>(FIX::Dictionary(settings->get(default_session_id)));
    
        sender_comp_id = settings->get().getString("SenderCompID");
        data_service_name = default_dictionary->getString("DataService");
                                                                                                             
                                                                                                
        auto participant_ptr =
            std::make_shared<distributed_ats_utils::basic_domain_participant>(0, sender_comp_id);

        participant_ptr->create_publisher();
        participant_ptr->create_subscriber();
      
      
        if (quickfix_config_file.empty()) {
            std::cerr << "Error: Config file name is not specified." << std::endl;
            return -1;
        }

      
        LOG4CXX_INFO(logger, "SenderCompID : [" << sender_comp_id << "] | DataService : ["<< data_service_name << "]");

        /* // content filtering for messages directed to instances of gateways
        // with given sender comp id.  These messages include Logon/Logout,
        // ExectionReports, MassStatus, etc.
        std::string target_comp_id_filter =
            "header.TargetCompID='" + sender_comp_id +
            "'";
      
        LOG4CXX_INFO(logger, "TargetCompID filter : [" << target_comp_id_filter << "]");
         */

        std::string fix_prefix = "FIXGateway-" + sender_comp_id + "." + data_service_name;
        
        FIX::FileStoreFactory store_factory(dats_log_home);
        DistributedATS::FileLogFactory log_factory(*settings, fix_prefix);

        auto data_writer_container =
            std::make_shared<DistributedATS::DataWriterContrainer>(participant_ptr);
      
        DistributedATS::DATSApplication application(data_service_name, sender_comp_id, data_writer_container);
        
        std::shared_ptr<DistributedATS::SocketAcceptor> acceptor;
        std::shared_ptr<FIX::SessionFactory> session_factory;
      
        try {
          
          acceptor = std::make_shared<DistributedATS::SocketAcceptor>( application, store_factory, *settings, log_factory);
          session_factory = std::make_shared<FIX::SessionFactory>( application, store_factory, &log_factory);
     
        } catch ( const std::exception& exp )
        {
          LOG4CXX_ERROR(logger, "Exception during the initialization of FIX Gateway : [" << exp.what() << "]");
        }
      
        auto data_reader_container =
        std::make_shared<DistributedATS::DataReaderContrainer>(participant_ptr, application, sender_comp_id);
      
        auto authService = std::make_shared<AuthServiceHelper>( settings, session_factory, default_dictionary, sender_comp_id);
        application.setAuthService(authService);

        acceptor->start();

        boost::asio::io_context io_service;
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
      
        signals.async_wait([&](const boost::system::error_code& ec, int signal_number) {
          if (!ec) {
              LOG4CXX_INFO(logger, "Signal number " << signal_number
                           << "Gracefully stopping the timer and exiting");
              is_running.store(false);
          } else {
              LOG4CXX_ERROR(logger, "Error " << ec.value() << " - " << ec.message()
                            << " - Signal number - " << signal_number);
          }
      });
      
      io_service.run();
      
      acceptor->stop();

      return 0;
      
  } catch (std::exception &e) {
      
      LOG4CXX_ERROR(logger, "Exception during the initialization of FIX Gateway : [" << e.what() << "]");

      return 1;
  }
}
