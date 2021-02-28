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

#include "SecurityListDataReaderListenerImpl.h"
#include <SecurityListLogger.hpp>

#include <LoggerHelper.h>

namespace DistributedATS {

SecurityListDataReaderListenerImpl::SecurityListDataReaderListenerImpl(
    MarketPtr marketPtr)
    : _marketPtr(marketPtr) {
  // TODO Auto-generated constructor stub
}

SecurityListDataReaderListenerImpl::~SecurityListDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
}

void SecurityListDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_SecurityList::SecurityListDataReader_var security_list_dr =
      DistributedATS_SecurityList::SecurityListDataReader::_narrow(reader);

    if (CORBA::is_nil(security_list_dr.in())) {
      std::cerr << "SecurityListDataReaderListenerImpl::on_data_available: "
                   "_narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_SecurityList::SecurityList security_list;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          security_list_dr->take_next_sample(security_list, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

        LoggerHelper::log_debug<std::stringstream, SecurityListLogger,
          DistributedATS_SecurityList::SecurityList>(
            security_list, "SecurityListLogger");

        for (uint32_t sec_index = 0;
             sec_index < security_list.c_NoRelatedSym.length(); sec_index++) {
          std::string instrument =
              security_list.c_NoRelatedSym[sec_index].Symbol.in();

          std::cout << "Adding book : " << instrument << std::endl;

            _marketPtr->addBook(instrument, true);
        }

        // request to recieve opening price
        _marketPtr->publishMarketDataRequest();

      } else if (status == DDS::RETCODE_NO_DATA) {
        break;
      } else {
        std::cerr << "ERROR: read DATS::SecurityList: Error: " << status
                  << std::endl;
      }
    }

  } catch (CORBA::Exception &e) {
    std::cerr << "Exception caught in read:" << std::endl << e << std::endl;
    ACE_OS::exit(1);
  }
}

} /* namespace DistributedATS */
