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

#include "LogonDataReaderListenerImpl.hpp"
#include <LogonAdapter.hpp>
#include <LogonLogger.hpp>
#include <LogonTypeSupportImpl.h>
#include <quickfix/Message.h>

namespace DistributedATS {

void LogonDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_Logon::LogonDataReader_var logon_dr =
      DistributedATS_Logon::LogonDataReader::_narrow(reader);

    if (CORBA::is_nil(logon_dr.in())) {
      std::cerr
          << "LogonDataReaderListenerImpl::on_data_available: _narrow failed."
          << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_Logon::Logon logon;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status = logon_dr->take_next_sample(logon, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

        std::stringstream ss;
        LogonLogger::log(ss, logon);
        ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Data Reader Logon : %s\n"),
                   ss.str().c_str()));

        FIX::Message logonMessage;
        logon.m_Header.SendingTime = 0; // this is precision;
        std::string logonSenderCompID = logon.m_Header.TargetSubID.in();

        logon.m_Header.SenderCompID = logon.m_Header.TargetCompID;
        logon.m_Header.TargetCompID = logonSenderCompID.c_str();

        LogonAdapter::DDS2FIX(logon, logonMessage);
        _application.processDDSLogon(logonMessage);

      } else if (status == DDS::RETCODE_NO_DATA) {
        break;
      } else {
        std::cerr << "ERROR: read DATS::Logon: Error: " << status << std::endl;
      }
    }

  } catch (CORBA::Exception &e) {
    std::cerr << "Exception caught in read:" << std::endl << e << std::endl;
    ACE_OS::exit(1);
  }
}
}; // namespace DistributedATS
