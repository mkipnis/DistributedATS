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


#include <MarketDataIncrementalRefreshAdapter.hpp>
#include <MarketDataIncrementalRefreshLogger.hpp>
#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <quickfix/fix50/MarketDataIncrementalRefresh.h>


#include "Application.hpp"

namespace DistributedATS {

auto const market_data_refresh_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& marketDataRefresh)
{
        FIX::Message marketDataIncementalRefreshMessage;

        marketDataRefresh.header().SendingTime(0); // this is precision;
        HeaderAdapter::DDS2FIX(marketDataRefresh.header(),
                               marketDataIncementalRefreshMessage.getHeader());

        for (int incremental_update = 0;
             incremental_update < marketDataRefresh.c_NoMDEntries().size();
             incremental_update++) {
            DistributedATS_MarketDataIncrementalRefresh::NoMDEntries mdEntry =
              marketDataRefresh.c_NoMDEntries()[incremental_update];

          if (mdEntry.MDUpdateAction() == 0)
            continue;

          FIX50::MarketDataIncrementalRefresh::NoMDEntries fixMDEntry;

          FIX::MDUpdateAction updateAction(mdEntry.MDUpdateAction());
          FIX::Symbol symbol(mdEntry.Symbol());
          FIX::SecurityExchange securityExchange(mdEntry.SecurityExchange());
          FIX::MDEntryType entryType(mdEntry.MDEntryType());
          FIX::MDEntryPx entryPx(mdEntry.MDEntryPx());
          FIX::MDEntrySize entrySize(mdEntry.MDEntrySize());

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


MarketDataIncrementalRefreshDataReaderListenerImpl::MarketDataIncrementalRefreshDataReaderListenerImpl(DistributedATS::DATSApplication &application) :
    _processor(application, market_data_refresh_processor, "MarketDataIncrementalRefreshDataReaderListenerImpl" )
{
}


void MarketDataIncrementalRefreshDataReaderListenerImpl::on_data_available(
                                eprosima::fastdds::dds::DataReader* reader)
{
    
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
      marketDataRefresh;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&marketDataRefresh, &info) == ReturnCode_t::RETCODE_OK)
    {
         if (info.valid_data)
         {
             _processor.enqueue_dds_message(marketDataRefresh);
         }
     }
}

} // namespace DistributedATS
