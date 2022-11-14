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

#include "MarketDataSnapshotFullRefreshDataReaderListenerImpl.h"
#include <MarketDataSnapshotFullRefreshAdapter.hpp>
#include <MarketDataSnapshotFullRefreshLogger.hpp>
#include <quickfix/fix50/MarketDataSnapshotFullRefresh.h>

#include "Application.hpp"

namespace DistributedATS {

auto const market_data_full_snapshot_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh&
                                               marketDataSnapshotFullRefresh)
{

    std::stringstream ss;
    MarketDataSnapshotFullRefreshLogger::log(ss,
                                             marketDataSnapshotFullRefresh);
    ACE_DEBUG(
        (LM_INFO,
         ACE_TEXT("(%P|%t) DATSMarketDataSnapshotFullRefreshLogger : %s\n"),
         ss.str().c_str()));

    FIX::Message marketDataSnapshotFullRefreshMessage;

    marketDataSnapshotFullRefresh.m_Header.SendingTime =
        0; // this is precision;
    marketDataSnapshotFullRefresh.m_Header.SenderCompID =
        marketDataSnapshotFullRefresh.m_Header.TargetSubID;

    HeaderAdapter::DDS2FIX(
        marketDataSnapshotFullRefresh.m_Header,
        marketDataSnapshotFullRefreshMessage.getHeader());

    FIX::Symbol symbol(marketDataSnapshotFullRefresh.Symbol.in());
    FIX::SecurityExchange securityExchange(
        marketDataSnapshotFullRefresh.SecurityExchange.in());

    marketDataSnapshotFullRefreshMessage.setField(symbol);
    marketDataSnapshotFullRefreshMessage.setField(securityExchange);

    for (int snapshot_refresh = 0;
         snapshot_refresh <
         marketDataSnapshotFullRefresh.c_NoMDEntries.length();
         ++snapshot_refresh) {
        DistributedATS_MarketDataSnapshotFullRefresh::NoMDEntries mdEntry =
          marketDataSnapshotFullRefresh.c_NoMDEntries[snapshot_refresh];

      FIX50::MarketDataSnapshotFullRefresh::NoMDEntries fixMDEntry;

      FIX::MDEntryType entryType(mdEntry.MDEntryType);
      FIX::MDEntryPx entryPx(mdEntry.MDEntryPx);
      FIX::MDEntrySize entrySize(mdEntry.MDEntrySize);

      fixMDEntry.setField(entryType);
      fixMDEntry.setField(entryPx);
      fixMDEntry.setField(entrySize);

      marketDataSnapshotFullRefreshMessage.addGroup(fixMDEntry);
    }

    DistributedATS::DATSApplication::publishToClient(
        marketDataSnapshotFullRefreshMessage);
};



MarketDataSnapshotFullRefreshDataReaderListenerImpl::MarketDataSnapshotFullRefreshDataReaderListenerImpl(DistributedATS::DATSApplication &application) : _processor(application, market_data_full_snapshot_processor )
{
}


void MarketDataSnapshotFullRefreshDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
  try {
      DistributedATS_MarketDataSnapshotFullRefresh::
        MarketDataSnapshotFullRefreshDataReader_var
            market_data_snapshot_full_refresh_dr =
      DistributedATS_MarketDataSnapshotFullRefresh::
                    MarketDataSnapshotFullRefreshDataReader::_narrow(reader);

    if (CORBA::is_nil(market_data_snapshot_full_refresh_dr.in())) {
      std::cerr << "DATSMarketDataSnapshotFullRefreshDataReaderListenerImpl::"
                   "on_data_available: _narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh
          marketDataSnapshotFullRefresh;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          market_data_snapshot_full_refresh_dr->take_next_sample(
              marketDataSnapshotFullRefresh, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;

          _processor.enqueue_dds_message(marketDataSnapshotFullRefresh);

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
