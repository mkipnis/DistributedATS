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
#include "Application.hpp"
#include <SecurityListAdapter.hpp>
#include <SecurityListLogger.hpp>
#include <quickfix/fix44/SecurityList.h>

namespace DistributedATS {

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
        DistributedATS_SecurityList::SecurityList securityList;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          security_list_dr->take_next_sample(securityList, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

        std::stringstream ss;
        SecurityListLogger::log(ss, securityList);
        ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) SecurityList : %s\n"),
                   ss.str().c_str()));

        FIX44::SecurityList securityListMessage;

        securityList.m_Header.SendingTime = 0; // this is precision;

        securityList.m_Header.SenderCompID =
            securityList.m_Header
                .TargetSubID; // security_list.m_Header.TargetCompID;
        HeaderAdapter::DDS2FIX(securityList.m_Header,
                               securityListMessage.getHeader());

        FIX::SecurityReqID securityReqID(securityList.SecurityReqID.in());
        securityListMessage.setField(securityReqID);

        FIX::SecurityResponseID securityResponseID(
            securityList.SecurityResponseID.in());
        securityListMessage.setField(securityResponseID);

        FIX::SecurityRequestResult securityRequestResult(
            securityList.SecurityRequestResult);
        securityListMessage.setField(securityRequestResult);

        for (int sec_index = 0;
             sec_index < securityList.c_NoRelatedSym.length(); sec_index++) {
          FIX44::SecurityList::NoRelatedSym relatedSymbol;

          std::string instrument =
              securityList.c_NoRelatedSym[sec_index].Symbol.in();
          std::string exchange =
              securityList.c_NoRelatedSym[sec_index].SecurityExchange.in();

	  std::string ref_data = securityList.c_NoRelatedSym[sec_index].Text.in();

          FIX::Symbol symbol(instrument);
          relatedSymbol.setField(symbol);

          FIX::SecurityExchange securityExchange(exchange);
          relatedSymbol.setField(securityExchange);

          FIX::Text text(ref_data);
          relatedSymbol.setField(text);

          securityListMessage.addGroup(relatedSymbol);
        }

        DistributedATS::DATSApplication::publishToClient(securityListMessage);

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
