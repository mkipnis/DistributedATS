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

#include <ExecutionReportAdapter.hpp>

#include <quickfix/Session.h>

#include "ExecutionReportDataReaderListenerImpl.h"

#include "Application.hpp"

#include <ExecutionReportLogger.hpp>

auto const exec_report_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_ExecutionReport::ExecutionReport& executionReport)
{
    std::stringstream ss;

    ExecutionReportLogger::log(ss, executionReport);

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Execution Report : %s\n"), ss.str().c_str()));

    FIX::Message executionReportMessage;
    executionReport.m_Header.SendingTime = 0; // this is precision; -- TODO: move to auto gen
    executionReport.m_Header.BeginString = CORBA::string_dup("FIX.4.4");
    executionReport.m_Header.SenderCompID = executionReport.m_Header.SenderSubID;
    ExecutionReportAdapter::DDS2FIX(executionReport, executionReportMessage);

    DATSApplication::publishToClient(executionReportMessage);
};


namespace DistributedATS {

ExecutionReportDataReaderListenerImpl::ExecutionReportDataReaderListenerImpl(DistributedATS::DATSApplication &application)
    : _processor(application, exec_report_processor, 100)
{
}

void ExecutionReportDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_ExecutionReport::ExecutionReportDataReader_var execution_report_dr =
      DistributedATS_ExecutionReport::ExecutionReportDataReader::_narrow(reader);

    if (CORBA::is_nil(execution_report_dr.in())) {
      std::cerr << "ExecutionReportDataReaderListenerImpl::on_data_available: "
                   "_narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_ExecutionReport::ExecutionReport executionReport;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          execution_report_dr->take_next_sample(executionReport, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;
          
          _processor.enqueue_dds_message(executionReport);

      } else if (status == DDS::RETCODE_NO_DATA) {
        break;
      } else {
        std::cerr << "ERROR: read DATS::ExecutionReport: Error: " << status
                  << std::endl;
      }
    }

  } catch (CORBA::Exception &e) {
    std::cerr << "Exception caught in read:" << std::endl << e << std::endl;
    ACE_OS::exit(1);
  }
}

} // namespace DistributedATS
