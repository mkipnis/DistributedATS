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

#ifndef __AUTHSERVICE_H__
#define __AUTHSERVICE_H__

#define HAVE_MYSQL


#include <ace/Guard_T.h>
#include <ace/RW_Mutex.h>
#include <memory>

#include <vector>

//#include <quickfix/Exceptions.h>
//#include <quickfix/DatabaseConnectionID.h>
//#include <quickfix/MySQLConnection.h>
#include <quickfix/DatabaseConnectionID.h>
#include "SQLiteConnection.hpp"


#include <iostream>

#include <ace/Task.h>
#include <ace/Task_T.h>

#include <BasicDomainParticipant.h>

#include <LogonTypeSupportImpl.h>
#include <LogoutTypeSupportImpl.h>



namespace DistributedATS {

class AuthService : public ACE_Task <ACE_MT_SYNCH>
{
public:

    
    AuthService(
    			std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
                const FIX::DatabaseConnectionID& dbConnectionID,
				ACE_Thread_Manager *thr_mgr
				);

    virtual ~AuthService();
    
    void createLogonTopic( const std::string& data_service_filter_expression );
    void createLogoutTopic( const std::string& data_service_filter_expression );
    
    virtual int svc (void);
    
private:

    /*
    bool authenticate( std::shared_ptr<FIX::MySQLConnection> mySqlConnect, DistributedATS_Logon::Logon* logonPtr );

    bool authenticate( std::shared_ptr<FIX::MySQLConnection> mySqlConnect, const char* username_in, const char* password_in, std::string& textOut );*/
    
    bool authenticate( std::shared_ptr<DistributedATS::SQLiteConnection> sqliteConnect, DistributedATS_Logon::Logon* logonPtr );

    bool authenticate( std::shared_ptr<DistributedATS::SQLiteConnection> sqliteConnect, const char* username_in, const char* password_in, std::string& textOut );

    FIX::DatabaseConnectionID m_dbConnectionID;

    std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> m_basicDomainParticipantPtr;
        
	std::map<std::string, std::string> m_credentials;
    
    DDS::ContentFilteredTopic_var m_cft_logon;
    DDS::ContentFilteredTopic_var m_cft_logout;
    
    DistributedATS_Logon::LogonDataWriter_var m_logon_dw;
    DistributedATS_Logout::LogoutDataWriter_var m_logout_dw;
    
};

} /* namespace DistributedATS */

#endif /* AUTHSERVICE_H_ */
