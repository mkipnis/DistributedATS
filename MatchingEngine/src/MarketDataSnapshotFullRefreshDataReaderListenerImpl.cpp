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

#include <LoggerHelper.h>

namespace DistributedATS {

MarketDataSnapshotFullRefreshDataReaderListenerImpl::
    ~MarketDataSnapshotFullRefreshDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
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

        LoggerHelper::log_debug<
            std::stringstream, MarketDataSnapshotFullRefreshLogger,
          DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh>(
            marketDataSnapshotFullRefresh, "MarketDataSnapshotFullRefresh");

        for (int index = 0;
             index < marketDataSnapshotFullRefresh.c_NoMDEntries.length();
             index++) {
          {
            std::string symbol = marketDataSnapshotFullRefresh.Symbol.in();

            _market->set_market_price(
                symbol,
                marketDataSnapshotFullRefresh.c_NoMDEntries[index].MDEntryPx);
          }
        }

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
