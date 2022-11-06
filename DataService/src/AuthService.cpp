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
#include <LogonTypeSupportImpl.h>
#include "LogonDataReaderListenerImpl.hpp"

#include "AuthService.h"
#include <sstream>

#include <quickfix/FixValues.h>
#include <ace/Time_Value.h>

#include <LogonLogger.hpp>
#include <LogoutLogger.hpp>

#include <Common.h>


using namespace DistributedATS;

AuthService::AuthService(std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
                         const FIX::DatabaseConnectionID& dbConnectionID,
						 ACE_Thread_Manager *thr_mgr ) : ACE_Task<ACE_MT_SYNCH> (thr_mgr), m_dbConnectionID( dbConnectionID )
{
    m_basicDomainParticipantPtr = basicDomainParticipantPtr;
};

AuthService::~AuthService()
{
    
}





void AuthService::createLogonTopic( const std::string& data_service_filter_expression/*,
                              const char* logon_topic_name, const char* logout_topic_name*/ )
{
    DDS::Topic_var logon_topic =
    m_basicDomainParticipantPtr->createTopicAndRegisterType < DistributedATS_Logon::LogonTypeSupport_var, DistributedATS_Logon::LogonTypeSupportImpl >
    ( LOGON_TOPIC_NAME );

    
    std::string data_service_filter_expression_auth = data_service_filter_expression +
        " and m_Header.TargetCompID = 'AUTH'";
    
    m_cft_logon =
        m_basicDomainParticipantPtr->getDomainParticipant()->create_contentfilteredtopic( "LOGON_FILTER" , logon_topic,
                                        data_service_filter_expression_auth.c_str(), DDS::StringSeq()); ;
    
    m_logon_dw = m_basicDomainParticipantPtr->createDataWriter<
    DistributedATS_Logon::LogonDataWriter_var, DistributedATS_Logon::LogonDataWriter > ( logon_topic );

    DDS::DataReaderListener_var logonDataListener( new LogonDataReaderListenerImpl( msg_queue() ) );
    m_basicDomainParticipantPtr->createDataReaderListener ( m_cft_logon.in(), logonDataListener );
}


void AuthService::createLogoutTopic( const std::string& data_service_filter_expression/*,
                              const char* logon_topic_name, const char* logout_topic_name*/ )
{
    DDS::Topic_var logout_topic =
    m_basicDomainParticipantPtr->createTopicAndRegisterType < DistributedATS_Logout::LogoutTypeSupport_var, DistributedATS_Logout::LogoutTypeSupportImpl >
    ( LOGOUT_TOPIC_NAME );
    
    std::string data_service_filter_expression_auth = data_service_filter_expression +
    " and m_Header.TargetCompID = 'AUTH'";
    
    
    m_cft_logout = m_basicDomainParticipantPtr->getDomainParticipant()->create_contentfilteredtopic( "LOGOUT_FILTER", logout_topic,
                                                                        data_service_filter_expression_auth.c_str(),
                                                                        DDS::StringSeq()); ;
    
    
    m_logout_dw = m_basicDomainParticipantPtr->createDataWriter< DistributedATS_Logout::LogoutDataWriter_var,
    DistributedATS_Logout::LogoutDataWriter > ( logout_topic );
}

int AuthService::svc (void)
{
    std::shared_ptr<DistributedATS::SQLiteConnection> mySqlConnectionPtr = std::make_shared<DistributedATS::SQLiteConnection>( m_dbConnectionID );
    
	while(1)
	{
        if ( !mySqlConnectionPtr->connected() )
        {
            ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) ERROR: Auth Service is not connected to the database.\n")));
            return 0;
        };
        
        ACE_Message_Block* messageBlock = 0;

        ACE_Time_Value interval (0, 5000000);
        
        if ( this->getq (messageBlock, &interval) > -1 )
        {
            DistributedATS_Logon::Logon* logonPtr = (  DistributedATS_Logon::Logon* )messageBlock->rd_ptr();
            authenticate( mySqlConnectionPtr, logonPtr );
            messageBlock->release();
        } else {
            ACE_OS::sleep(interval);
        }
	};

	return 0;
}


bool AuthService::authenticate( std::shared_ptr<DistributedATS::SQLiteConnection> sqliteConnectionPtr,  DistributedATS_Logon::Logon* logonPtr )
{
    std::string textOut = "";
    
    if ( authenticate( sqliteConnectionPtr, logonPtr->Username.in(), logonPtr->Password.in(), textOut ) )
    {
        DistributedATS_Logon::Logon logon = *logonPtr;
        logon.m_Header.TargetCompID = logonPtr->m_Header.SenderCompID;
        logon.m_Header.SenderCompID = CORBA::string_dup("AUTH");
        logon.m_Header.TargetSubID = CORBA::string_dup( logonPtr->Username );
        
        std::stringstream ss_logon;
        LogonLogger::log(ss_logon, logon);
        ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Auth Service Logon Success: %s\n"), ss_logon.str().c_str()));

        int ret = m_logon_dw->write(logon, NULL);
        
        if (ret != DDS::RETCODE_OK)
        {
            ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) ERROR: Logon write returned %d.\n"), ret));
        }
    } else {
        
        ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t|%D) INFO: Authentication failed: Publishing logout [%s].\n"), logonPtr->Username.in()));
        
        DistributedATS_Logout::Logout logout;
        
        logout.m_Header.BeginString = logonPtr->m_Header.BeginString;
        logout.m_Header.MsgType = FIX::MsgType_Logout;
        
        logout.m_Header.TargetCompID = logonPtr->m_Header.SenderCompID;
        logout.m_Header.SenderCompID = CORBA::string_dup("AUTH");
        logout.m_Header.TargetSubID = logonPtr->RawData; // Kludge
        logout.m_Header.SendingTime = 0;
        
        logout.Text = CORBA::string_dup(textOut.c_str());

        std::stringstream ss_logout;
        LogoutLogger::log(ss_logout, logout);
        ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Auth Service Logout : %s\n"), ss_logout.str().c_str()));

        
        int ret = m_logout_dw->write(logout, NULL);
        
        if (ret != DDS::RETCODE_OK)
        {
            ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D)  ERROR: Logout write returned %d.\n"), ret));
        }
    }

    return true;
}


/*
bool AuthService::authenticate( std::shared_ptr<FIX::MySQLConnection> mySqlConnectionPtr, const char* username_in, const char* password_in, std::string& textOut )
{
    std::stringstream user_logon_call;
    
    
    user_logon_call << "call user_logon('" << username_in << "','" <<password_in << "', @usercheck, @reason)";

    std::cout << "user logon call : " <<user_logon_call.str() << std::endl;
    
    FIX::MySQLQuery mySQLQuery(user_logon_call.str().c_str());
    mySqlConnectionPtr->execute(mySQLQuery);
    
    FIX::MySQLQuery mySQLQueryOut("select @usercheck, @reason");
    mySqlConnectionPtr->execute(mySQLQueryOut);
    
    int result =  ACE_OS::atoi(mySQLQueryOut.getValue(0,0));
    textOut = mySQLQueryOut.getValue(0, 1);
    
    return result == 0 ? true : false;
};
 */


bool AuthService::authenticate( std::shared_ptr<DistributedATS::SQLiteConnection> sqliteConnect, const char* username_in, const char* password_in, std::string& textOut )
{
    std::stringstream auth_query_stream;
    
    auth_query_stream << "select * from user_code where user_name='" << username_in <<
                    "' and json_extract(properties, \"$.password\")='" << password_in << "'";
    
    DistributedATS::SQLiteQuery auth_query(auth_query_stream.str().c_str());
    
    sqliteConnect->execute(auth_query);
    
    if ( auth_query.rows() == 0 )
    {
        textOut = "Invalid username or password";
        return false;
    } else {
        return true;
    }
};

