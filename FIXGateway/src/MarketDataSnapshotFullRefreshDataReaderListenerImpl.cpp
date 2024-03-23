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

#include "MarketDataSnapshotFullRefreshDataReaderListenerImpl.h"
#include <MarketDataSnapshotFullRefreshAdapter.hpp>
#include <MarketDataSnapshotFullRefreshLogger.hpp>
#include <quickfix/fix50/MarketDataSnapshotFullRefresh.h>

#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "Application.hpp"

namespace DistributedATS {

auto const market_data_full_snapshot_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh&
                                               marketDataSnapshotFullRefresh)
{
    FIX::Message marketDataSnapshotFullRefreshMessage;

    marketDataSnapshotFullRefresh.header().SendingTime(0); // this is precision;
    marketDataSnapshotFullRefresh.header().SenderCompID(
        marketDataSnapshotFullRefresh.header().TargetSubID());

    HeaderAdapter::DDS2FIX(
        marketDataSnapshotFullRefresh.header(),
        marketDataSnapshotFullRefreshMessage.getHeader());

    FIX::Symbol symbol(marketDataSnapshotFullRefresh.Symbol());
    FIX::SecurityExchange securityExchange(
        marketDataSnapshotFullRefresh.SecurityExchange());

    marketDataSnapshotFullRefreshMessage.setField(symbol);
    marketDataSnapshotFullRefreshMessage.setField(securityExchange);

    for (int snapshot_refresh = 0;
         snapshot_refresh <
         marketDataSnapshotFullRefresh.c_NoMDEntries().size();
         ++snapshot_refresh) {
        DistributedATS_MarketDataSnapshotFullRefresh::NoMDEntries mdEntry =
          marketDataSnapshotFullRefresh.c_NoMDEntries()[snapshot_refresh];

      FIX50::MarketDataSnapshotFullRefresh::NoMDEntries fixMDEntry;

      FIX::MDEntryType entryType(mdEntry.MDEntryType());
      FIX::MDEntryPx entryPx(mdEntry.MDEntryPx());
      FIX::MDEntrySize entrySize(mdEntry.MDEntrySize());

      fixMDEntry.setField(entryType);
      fixMDEntry.setField(entryPx);
      fixMDEntry.setField(entrySize);

      marketDataSnapshotFullRefreshMessage.addGroup(fixMDEntry);
    }

    DistributedATS::DATSApplication::publishToClient(
        marketDataSnapshotFullRefreshMessage);
};



MarketDataSnapshotFullRefreshDataReaderListenerImpl::MarketDataSnapshotFullRefreshDataReaderListenerImpl(DistributedATS::DATSApplication &application) :
    _processor(application, market_data_full_snapshot_processor,
               "MarketDataSnapshotFullRefreshDataReaderListenerImpl" ),
        _fix_gateway_name(application.fix_gateway_name())
{
}


void MarketDataSnapshotFullRefreshDataReaderListenerImpl::on_data_available(
       eprosima::fastdds::dds::DataReader* reader)
{

    DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh
      marketDataSnapshotFullRefresh;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&marketDataSnapshotFullRefresh, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            if ( marketDataSnapshotFullRefresh.header().TargetCompID().compare(_fix_gateway_name) == 0 )
            {
                std::stringstream ss;
                MarketDataSnapshotFullRefreshLogger::log(ss, marketDataSnapshotFullRefresh);
                LOG4CXX_INFO(logger, "marketDataSnapshotFullRefresh : [" <<  ss.str() << "]");
                
                _processor.enqueue_dds_message(marketDataSnapshotFullRefresh);
            }
        }
    }
    
}

} /* namespace DistributedATS */
