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

#pragma once

#include <memory>
#include <vector>
#include <thread>

#include <quickfix/DatabaseConnectionID.h>
#include "SQLiteConnection.hpp"
#include <iostream>
#include <BasicDomainParticipant.h>
#include <Logon.hpp>
#include <Logout.hpp>
#include "LogonDataReaderListenerImpl.hpp"



namespace DistributedATS {

class AuthService
{
public:

    
    AuthService(
                std::shared_ptr<distributed_ats_utils::basic_domain_participant> basic_domain_participant_ptr,
                const FIX::DatabaseConnectionID& dbConnectionID
				);

    virtual ~AuthService();
    
    void createLogonTopic();
    void createLogoutTopic();
    
    int service (void);
    
private:
    
    bool authenticate(
                      std::shared_ptr<DistributedATS::SQLiteConnection>& sqliteConnect,
                      std::shared_ptr<DistributedATS_Logon::Logon>& logonPtr );

    bool authenticate( std::shared_ptr<DistributedATS::SQLiteConnection>& sqliteConnect,
                      const std::string& username_in,
                      const std::string& password_in,
                      std::string& textOut );

    FIX::DatabaseConnectionID m_dbConnectionID;

    distributed_ats_utils::basic_domain_participant_ptr _basic_domain_participant_ptr;
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_Logon::Logon> _logon_topic_tuple;
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_Logout::Logout> _logout_topic_tuple;
    
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_Logon::Logon> _logon_data_reader_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_Logout::Logout> _logout_data_reader_tuple;
        
	std::map<std::string, std::string> m_credentials;
    
    distributed_ats_utils::data_writer_ptr m_logon_dw;
    distributed_ats_utils::data_writer_ptr m_logout_dw;
    
    LogonPtrQueuePtr _logon_request_queue;
    
    std::thread _service_thread;
    
    std::atomic<bool> _is_running;

    
};

} /* namespace DistributedATS */
