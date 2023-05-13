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

#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"
#include <MarketDataIncrementalRefreshAdapter.hpp>
#include <MarketDataIncrementalRefreshLogger.hpp>

#include <quickfix/fix50/MarketDataIncrementalRefresh.h>

namespace DistributedATS {

auto const market_data_refresh_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& marketDataRefresh)
{
        FIX::Message marketDataIncementalRefreshMessage;

        marketDataRefresh.m_Header.SendingTime = 0; // this is precision;
        HeaderAdapter::DDS2FIX(marketDataRefresh.m_Header,
                               marketDataIncementalRefreshMessage.getHeader());

        for (int incremental_update = 0;
             incremental_update < marketDataRefresh.c_NoMDEntries.length();
             incremental_update++) {
            DistributedATS_MarketDataIncrementalRefresh::NoMDEntries mdEntry =
              marketDataRefresh.c_NoMDEntries[incremental_update];

          if (mdEntry.MDUpdateAction == 0)
            continue;

          FIX50::MarketDataIncrementalRefresh::NoMDEntries fixMDEntry;

          FIX::MDUpdateAction updateAction(mdEntry.MDUpdateAction);
          FIX::Symbol symbol(mdEntry.Symbol.in());
          FIX::SecurityExchange securityExchange(mdEntry.SecurityExchange.in());
          FIX::MDEntryType entryType(mdEntry.MDEntryType);
          FIX::MDEntryPx entryPx(mdEntry.MDEntryPx);
          FIX::MDEntrySize entrySize(mdEntry.MDEntrySize);

          fixMDEntry.setField(updateAction);
          fixMDEntry.setField(symbol);
          fixMDEntry.setField(securityExchange);
          fixMDEntry.setField(entryType);
          fixMDEntry.setField(entryPx);
          fixMDEntry.setField(entrySize);

          marketDataIncementalRefreshMessage.addGroup(fixMDEntry);
        }

        DistributedATS::DATSApplication::broadcastToClients(marketDataIncementalRefreshMessage);
};


MarketDataIncrementalRefreshDataReaderListenerImpl::MarketDataIncrementalRefreshDataReaderListenerImpl(DistributedATS::DATSApplication &application) : _processor(application, market_data_refresh_processor )
{
}


void MarketDataIncrementalRefreshDataReaderListenerImpl::on_data_available(
    DDS::DataReader_ptr reader) throw(CORBA::SystemException) {
        
        static int count = 0;
  try {
      DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshDataReader_var
        market_data_incremental_refresh_dr = DistributedATS_MarketDataIncrementalRefresh::
            MarketDataIncrementalRefreshDataReader::_narrow(reader);

    if (CORBA::is_nil(market_data_incremental_refresh_dr.in())) {
      std::cerr << "MarketDataIncrementalRefreshDataReaderListenerImpl::on_"
                   "data_available: _narrow failed."
                << std::endl;
      ACE_OS::exit(1);
    }

    while (true) {
        DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
          marketDataRefresh;
      DDS::SampleInfo si;
      DDS::ReturnCode_t status =
          market_data_incremental_refresh_dr->take_next_sample(
              marketDataRefresh, si);

      if (status == DDS::RETCODE_OK) {
        if (!si.valid_data)
          continue;
          
          _processor.enqueue_dds_message(marketDataRefresh);

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

} // namespace DistributedATS
