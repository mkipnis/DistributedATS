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

#include "LogoutDataReaderListenerImpl.hpp"
#include <LogoutAdapter.hpp>
#include <LogoutLogger.hpp>
#include <LogoutTypeSupportImpl.h>
#include <quickfix/Message.h>

namespace DistributedATS {


auto const logout_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_Logout::Logout& logout)
{
    std::stringstream ss;
    LogoutLogger::log(ss, logout);
    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Data Reader Logout : %s\n"),
               ss.str().c_str()));

    FIX::Message logoutMessage;
    logout.m_Header.SendingTime = 0;

    LogoutAdapter::DDS2FIX(logout, logoutMessage);

    std::string connectionToken =
        logout.m_Header.TargetSubID
            .in(); // Kludge : Connection token is populated in RawData in
                   // Logon message, but because there is no RawData in
                   // Logout message, DataService populates TargetSubID with
                   // the Connection Token of the session that needs to be
                   // Logged out and disconnected in case of invalid
                   // credentials.
    application.processDDSLogout(connectionToken, logoutMessage);
};



LogoutDataReaderListenerImpl::LogoutDataReaderListenerImpl(DistributedATS::DATSApplication &application)
    : _processor(application, logout_processor)
{
};


void LogoutDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_Logout::LogoutDataReader_var logout_dr =
      DistributedATS_Logout::LogoutDataReader::_narrow(reader);

    if (CORBA::is_nil(logout_dr.in())) {
      std::cerr
          << "LogoutDataReaderListenerImpl::on_data_available: _narrow failed."
          << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_Logout::Logout logout;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status = logout_dr->take_next_sample(logout, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;
          
          _processor.enqueue_dds_message(logout);

      } else if (status == DDS::RETCODE_NO_DATA) {
        break;
      } else {
        std::cerr << "ERROR: read DATS::Logout: Error: " << status << std::endl;
      }
    }

  } catch (CORBA::Exception &e) {
    std::cerr << "Exception caught in read:" << std::endl << e << std::endl;
    ACE_OS::exit(1);
  }
}
}; // namespace DistributedATS
