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
#include <BasicDomainParticipant.h>
#include "AuthService.h"
#include "RefDataService.h"
#include "MarketDataService.h"
#include "OrderMassStatusRequestService.h"
#include "SQLiteConnection.hpp"
#include <memory>
#include <quickfix/FixValues.h>
#include <Common.h>

#include <atomic>

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

std::atomic<bool> is_running;


int main(int argc, char* argv[] )
{
    LOG4CXX_INFO(logger, "DataService");
    
    try {

        if ( argc < 2 )
        {
            std::cout << "usage: " << argv[ 0 ] << std::endl << "\t\t-c data_service_config_file_name" << std::endl;
            return -1;
        }

        std::string data_service_config_file = "";
        
        boost::program_options::options_description options_desc{"Options"};
        
        options_desc.add_options()
          ("help,h", "Help screen")
          ("config,c", boost::program_options::value<std::string>()->default_value(""), "DataService ConfigFile");
        
        boost::program_options::variables_map vm;
        boost::program_options::store(parse_command_line(argc, argv, options_desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
          std::cout << options_desc << '\n';
        else if (vm.count("config"))
            data_service_config_file = vm["config"].as<std::string>();
        
        
        distributed_ats_utils::basic_domain_participant_ptr basic_domain_participant_ptr =
            std::make_shared<distributed_ats_utils::basic_domain_participant>( 0, "DataService" );
        
        basic_domain_participant_ptr->create_subscriber();
        basic_domain_participant_ptr->create_publisher();


        if ( data_service_config_file.empty() )
        {
            std::cerr << "Error: Config file name is not specified." << std::endl;
            return -1;
        }
        
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(data_service_config_file, pt);
        
        auto base_dir_ats = std::getenv("BASEDIR_ATS");
      
        if ( base_dir_ats == NULL )
            throw std::runtime_error("BASEDIR_ATS is not set");
        
        std::string data_service_name = pt.get<std::string>("dataservice.name");
        std::string database_file = pt.get<std::string>("database.database_file");

        FIX::DatabaseConnectionID databaseConnectionID(std::string(base_dir_ats) + "/data/" + database_file,"", "", "", 0);
        
        std::shared_ptr<DistributedATS::SQLiteConnection> sql_connection =
            std::make_shared<DistributedATS::SQLiteConnection>( databaseConnectionID );
        
        if ( !sql_connection->connected() )
        {
            LOG4CXX_ERROR(logger, "Data Service is not connected to the database");
            return -1;
        };
        
        auto authServicePtr =
                std::make_shared<DistributedATS::AuthService>(basic_domain_participant_ptr, databaseConnectionID );
        
        auto refServicePtr =
                std::make_shared<DistributedATS::RefDataService>(basic_domain_participant_ptr, databaseConnectionID);
        
        auto marketDataServicePtr =
                std::make_shared<DistributedATS::MarketDataService>(basic_domain_participant_ptr, databaseConnectionID);
        
       auto orderMassStatusServicePtr =
                std::make_shared<DistributedATS::OrderMassStatusRequestService>( basic_domain_participant_ptr );
        
        std::string data_service_filter_expression = "header.TargetSubID = '" + data_service_name + "'";
        
        // Ref Data
        refServicePtr->createSecurityListRequestListener(data_service_filter_expression);
        refServicePtr->createSecurityListDataWriter();
        
        // Authentication
        authServicePtr->createLogonTopic(data_service_filter_expression);
        authServicePtr->createLogoutTopic(data_service_filter_expression);
        
        
        // Market Data
        marketDataServicePtr->createMarketDataRequestListener(data_service_name);
        marketDataServicePtr->createMarketDataIncrementalRefreshListener();
        marketDataServicePtr->createMarketDataFullRefreshDataWriter();
        
        // Order Status
        orderMassStatusServicePtr->createOrderMassStatusRequestListener(data_service_filter_expression);
        orderMassStatusServicePtr->createExecutionReportListener(data_service_filter_expression);
        
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
        
    } catch ( std::exception & e) {
        
        LOG4CXX_ERROR(logger, "Exception during the initialization of Data Service" << e.what());
    	return 1;
    }
    
};
