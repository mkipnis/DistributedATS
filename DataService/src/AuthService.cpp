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
#include "AuthService.h"
#include <sstream>
#include <LogonPubSubTypes.h>
#include <LogoutPubSubTypes.h>
#include <quickfix/FixValues.h>
#include <LogonLogger.hpp>
#include <LogoutLogger.hpp>
#include <Common.h>
#include <thread>
#include <chrono>
#include <tuple>


using namespace DistributedATS;

AuthService::AuthService(std::shared_ptr<distributed_ats_utils::basic_domain_participant> basic_domain_participant_ptr,
                         const FIX::DatabaseConnectionID& dbConnectionID) : m_dbConnectionID( dbConnectionID )
{
    _basic_domain_participant_ptr = basic_domain_participant_ptr;
    _logon_request_queue = std::make_shared<LogonPtrQueue>();
    
    std::atomic_init(&_is_running, true);

    _service_thread = std::thread(&AuthService::service, this);
};

AuthService::~AuthService()
{
    std::atomic_init(&_is_running, false);
    _service_thread.join();
}

void AuthService::createLogonTopic( const std::string& data_service_filter_expression )
{
    _logon_topic_tuple =
    _basic_domain_participant_ptr->make_topic < DistributedATS_Logon::LogonPubSubType, DistributedATS_Logon::Logon  >
    ( LOGON_TOPIC_NAME );

    
    std::string data_service_filter_expression_auth = data_service_filter_expression +
        "and header.TargetCompID='AUTH'";
    
    _logon_data_reader_tuple = _basic_domain_participant_ptr->make_data_reader_tuple(_logon_topic_tuple,
                new LogonDataReaderListenerImpl( _logon_request_queue ),
                "FILTERED_LOGON", data_service_filter_expression_auth);

    m_logon_dw = _basic_domain_participant_ptr->make_data_writer<DistributedATS_Logon::Logon>( _logon_topic_tuple );
}


void AuthService::createLogoutTopic( const std::string& data_service_filter_expression )
{
    _logout_topic_tuple =
    _basic_domain_participant_ptr->make_topic < DistributedATS_Logout::LogoutPubSubType, DistributedATS_Logout::Logout >
    ( LOGOUT_TOPIC_NAME );
    
    std::string data_service_filter_expression_auth = data_service_filter_expression +
    " and header.TargetCompID = 'AUTH'";
    
    m_logout_dw = _basic_domain_participant_ptr->make_data_writer( _logout_topic_tuple );
}

int AuthService::service (void)
{
    std::shared_ptr<DistributedATS::SQLiteConnection> sqlite_connection_ptr = std::make_shared<DistributedATS::SQLiteConnection>( m_dbConnectionID );
    
	while(_is_running)
	{
        if ( !sqlite_connection_ptr->connected() )
        {
            LOG4CXX_ERROR(logger, "Auth Service is not connected to the database.\n");
            
            return 0;
        };
        
        std::shared_ptr<DistributedATS_Logon::Logon> logon_ptr;
        
        if ( _logon_request_queue->pop( logon_ptr ) )
        {
            authenticate( sqlite_connection_ptr, logon_ptr );
        } else {
            std::this_thread::sleep_for(std::chrono::duration<long double, std::milli>(1000));
        }
	};

	return 0;
}


bool AuthService::authenticate( std::shared_ptr<DistributedATS::SQLiteConnection>& sqliteConnectionPtr,  std::shared_ptr<DistributedATS_Logon::Logon>& logon )
{
    std::string textOut = "";
    
    std::stringstream ss_logon;
    LogonLogger::log(ss_logon, *logon);
    
    if ( authenticate( sqliteConnectionPtr, logon->Username(), logon->Password(), textOut ) )
    {
        logon->header().TargetCompID(logon->header().SenderCompID());
        logon->header().SenderCompID("AUTH");
        logon->header().TargetSubID( logon->Username() );
        
        LOG4CXX_INFO(logger, "Auth Service Logon Success: [" <<  ss_logon.str() << "]");
        
        int ret = m_logon_dw->write(logon.get());
        
        if (!ret) {
            LOG4CXX_ERROR(logger, "Error Publishing to DDS :" << ss_logon.str());
        }
        
    } else {
        
        LOG4CXX_INFO(logger, "Authentication failed: Publishing logout for : [" <<  ss_logon.str() << "]");

        DistributedATS_Logout::Logout logout;
        
        logout.header().BeginString(logon->header().BeginString());
        logout.header().MsgType(FIX::MsgType_Logout);
        
        logout.header().TargetCompID(logon->header().SenderCompID());
        logout.header().SenderCompID("AUTH");
        logout.header().TargetSubID(logon->RawData());
        logout.header().SendingTime(0);
        
        logout.Text(textOut);

        std::stringstream ss_logout;
        LogoutLogger::log(ss_logout, logout);
        LOG4CXX_INFO(logger, "Auth Service Logout : %s\n" << ss_logout.str());
        
        int ret = m_logout_dw->write(&logout);
        
        if (!ret) {
            LOG4CXX_ERROR(logger, "Logout write returned error : " << ret );
        }
    }

    return true;
}



bool AuthService::authenticate( std::shared_ptr<DistributedATS::SQLiteConnection>& sqliteConnect, const std::string& username, const std::string& password, std::string& textOut )
{
    std::stringstream auth_query_stream;
    
    auth_query_stream << "select * from user_code where user_name='" << username <<
                    "' and json_extract(properties, \"$.password\")='" << password << "'";
    
    DistributedATS::SQLiteQuery auth_query(auth_query_stream.str());
    
    sqliteConnect->execute(auth_query);
    
    if ( auth_query.rows() == 0 )
    {
        textOut = "Invalid username or password";
        return false;
    } else {
        return true;
    }
};

