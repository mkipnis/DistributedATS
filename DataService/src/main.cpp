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

#include <iostream>
#include <BasicDomainParticipant.h>
#include <ConfigFileHelper.h>
#include <ace/Get_Opt.h>
#include "AuthService.h"
#include "RefDataService.h"
#include "MarketDataService.h"
#include "MassOrderStatusService.h"
#include <quickfix/DatabaseConnectionID.h>
#include <memory>
#include <ace/Message_Queue_T.h>
#include <quickfix/FixValues.h>
#include <Common.h>

#include <atomic>

std::atomic<bool> is_running;

namespace DistributedATS {

class SignalHandler :  public ACE_Event_Handler
{
public:
    virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0)
    {
        std::cout << "Lets hangle signal : " << signum << std::endl;
        is_running.store(false);
        return 0;
    };
    
};

};


int main(int argc, char* argv[] )
{
    
    ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t) INFO: DataService\n")));
    
    DDS::DomainParticipantFactory_var dpf = DDS::DomainParticipantFactory::_nil();

    try {

	if ( argc < 2 )
	{
		std::cout << "usage: " << argv[ 0 ] << std::endl << "\t\t-c data_service_config_file_name" << std::endl;
		return -1;
	}

        std::string data_service_config_file = "";
        
        dpf = TheParticipantFactoryWithArgs(argc, argv);
        
        distributed_ats_utils::BasicDomainParticipantPtr participantPtr =
            std::make_shared<distributed_ats_utils::BasicDomainParticipant>( dpf, DISTRIBUTED_ATS_DOMAIN_ID );
        
        participantPtr->createSubscriber();
        participantPtr->createPublisher();

        ACE_Get_Opt cmd_opts( argc, argv, ":c:" );

        int option;

        while ( (option = cmd_opts()) != EOF )
        {
            switch( option )
            {
                case 'c' :
                	data_service_config_file = cmd_opts.opt_arg();
                break;
            }
        }

	if ( data_service_config_file.empty() )
	{
		std::cerr << "Error: Config file name is not specified." << std::endl;
		return -1;
	}
        
        ACE_Sig_Handler handler;
        DistributedATS::SignalHandler signalHandler;
        
        // add signal handler for the SIGINT signal here
        ACE_Sig_Handler sig_handler;
        sig_handler.register_handler (SIGINT, &signalHandler);

        ACE_DEBUG (( LM_INFO, ACE_TEXT("(%P|%t|%D) INFO: Config File : [%s]\n"), data_service_config_file.c_str() ));

        ConfigFileHelper configFileHelper(data_service_config_file.c_str());

     	ACE_TString data_service_name;
     	if (configFileHelper.get_string_value (ACE_TEXT ("dataservice"), ACE_TEXT ("name"), data_service_name))
     	{
            ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Unable to get data service name from ini file %d.\n"), ACE_OS::last_error() ));
     	};

        ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t|%D) INFO: DataService:[%s]\n"), data_service_name.c_str()));


     	int port = 0;
     	if (configFileHelper.get_integer_value (ACE_TEXT ("database"), ACE_TEXT ("portnumber"), &port))
     	{
            ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Unable to get database port from ini file %d.\n"), ACE_OS::last_error() ));
     	};

     	ACE_TString hostname;
     	if (configFileHelper.get_string_value (ACE_TEXT ("database"), ACE_TEXT ("hostname"), hostname))
     	{
            ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Unable to get database hostname from ini file %d.\n"), ACE_OS::last_error() ));
     	};

     	ACE_TString username;
     	if (configFileHelper.get_string_value (ACE_TEXT ("database"), ACE_TEXT ("username"), username))
     	{
            ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Unable to get database username from ini file %d.\n"), ACE_OS::last_error() ));
     	};

     	ACE_TString password;
     	if (configFileHelper.get_string_value (ACE_TEXT ("database"), ACE_TEXT ("password"), password))
     	{
            ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Unable to get database password from ini file %d.\n"), ACE_OS::last_error() ));
     	};

     	ACE_TString database;
     	if (configFileHelper.get_string_value (ACE_TEXT ("database"), ACE_TEXT ("database"), database))
     	{
            ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Unable to get database password from ini file %d.\n"), ACE_OS::last_error() ));
     	};
        
        ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t|%D) INFO: Database settings : Hostname : [%s]|Port : [%d]|Username : [%s]|Password : [%s]|Database : [%s]\n"), hostname.c_str(),
                   port, username.c_str(), password.c_str(), database.c_str()));


     	FIX::DatabaseConnectionID databaseConnectionID(database.c_str(), username.c_str(), password.c_str(), hostname.c_str(), port);
        
        std::shared_ptr<FIX::MySQLConnection> mySqlConnectionPtr =
            std::make_shared<FIX::MySQLConnection>( databaseConnectionID );
        
        if ( !mySqlConnectionPtr->connected() )
        {
                ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Data Service is not connected to the database.\n")));
                return -1;
        };
        
        auto authServicePtr =
                std::make_shared<DistributedATS::AuthService>(participantPtr, databaseConnectionID, ACE_Thread_Manager::instance () );
        
        auto refServicePtr =
                std::make_shared<DistributedATS::RefDataService>(participantPtr, databaseConnectionID, ACE_Thread_Manager::instance ());
        
        auto marketDataServicePtr =
                std::make_shared<DistributedATS::MarketDataService>(participantPtr, databaseConnectionID, ACE_Thread_Manager::instance ());
        
       auto massOrderStatusServicePtr =
                std::make_shared<DistributedATS::MassOrderStatusService>( participantPtr, ACE_Thread_Manager::instance ());
        
        std::string data_service_filter_expression = "m_Header.TargetSubID = '" + std::string( data_service_name.c_str() ) + "'";
        
        // Authentication
        authServicePtr->createLogonTopic(data_service_filter_expression);
        authServicePtr->createLogoutTopic(data_service_filter_expression);
        
        // Ref Data
        refServicePtr->createSecurityListRequestListener(data_service_filter_expression);
        refServicePtr->createSecurityListDataWriter();
        
        // Market Data
        marketDataServicePtr->createMarketDataRequestListener(data_service_filter_expression);
        marketDataServicePtr->createMarketDataIncrementalRefreshListener();
        marketDataServicePtr->createMarketDataFullRefreshDataWriter();
        
        // Order Status
        massOrderStatusServicePtr->createOrderMassStatusRequestListener(data_service_filter_expression);
        massOrderStatusServicePtr->createExecutionReportListener(data_service_filter_expression);
        
        authServicePtr->activate(THR_NEW_LWP, 2); // since we will be making a blocking database call, lets hava a pool of 2
        refServicePtr->activate();
        marketDataServicePtr->activate();
        massOrderStatusServicePtr->activate();
        
        std::atomic_init(&is_running, true);

        while ( is_running == true )
            ACE_OS::sleep(1);
        
    } catch ( std::exception & e) {
        
        ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Exception during the initialization of Data Service : %s\n"), e.what()));
        
    	std::cout << e.what() << std::endl;
    	return 1;
    }
    
    TheServiceParticipant->shutdown ();
};
