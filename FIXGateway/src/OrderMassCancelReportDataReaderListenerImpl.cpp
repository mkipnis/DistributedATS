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

#include "OrderMassCancelReportDataReaderListenerImpl.h"
#include "Application.hpp"
#include <OrderMassCancelReportAdapter.hpp>
#include <OrderMassCancelReportLogger.hpp>

namespace DistributedATS {

/*OrderCancelReportDataReaderListenerImpl::OrderCancelReportDataReaderListenerImpl()
{
        // TODO Auto-generated constructor stub

}*/

OrderMassCancelReportDataReaderListenerImpl::
    ~OrderMassCancelReportDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
}

void OrderMassCancelReportDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataReader_var
        order_mass_cancel_report_dr =
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataReader::_narrow(
                reader);

    if (CORBA::is_nil(order_mass_cancel_report_dr.in())) {
      std::cerr << "OrderCancelReportDataReaderListenerImpl::on_data_available:"
                   " _narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_OrderMassCancelReport::OrderMassCancelReport orderMassCancelReport;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status = order_mass_cancel_report_dr->take_next_sample(
          orderMassCancelReport, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

        std::stringstream ss;
        OrderMassCancelReportLogger::log(ss, orderMassCancelReport);
        ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) OrderMassCancelReport : %s\n"),
                   ss.str().c_str()));

        FIX::Message orderMassCancelReportMessage;

        orderMassCancelReport.m_Header.BeginString =
            CORBA::string_dup("FIX.4.4");
        orderMassCancelReport.m_Header.SendingTime = 0; // this is precision;
        orderMassCancelReport.m_Header.SenderCompID =
            orderMassCancelReport.m_Header.TargetSubID;

        OrderMassCancelReportAdapter::DDS2FIX(orderMassCancelReport,
                                              orderMassCancelReportMessage);

        DistributedATS::DATSApplication::publishToClient(
            orderMassCancelReportMessage);

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

} /* namespace DistributedATS */
