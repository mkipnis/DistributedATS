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

#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"
#include <MarketDataIncrementalRefreshAdapter.hpp>
#include <quickfix/fix50/MarketDataIncrementalRefresh.h>
#include <MarketDataIncrementalRefreshLogger.hpp>

#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <sstream>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>


namespace DistributedATS {


void MarketDataIncrementalRefreshDataReaderListenerImpl::on_data_available(eprosima::fastdds::dds::DataReader* reader) 
{
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh marketDataRefresh;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&marketDataRefresh, &info) == eprosima::fastdds::dds::RETCODE_OK)
    {
        if (info.valid_data)
        {
            std::stringstream ss;
            MarketDataIncrementalRefreshLogger::log(ss, marketDataRefresh);
            LOG4CXX_INFO(logger, "MarketDataIncrementalRefresh : [" <<  ss.str() << "]");

            insertIncrementalMarketDataRefresh(marketDataRefresh);
                
        }
    }

}
    
    
    
void MarketDataIncrementalRefreshDataReaderListenerImpl::insertIncrementalMarketDataRefresh( const DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& incrementalRefresh )
{
    
    std::map<Instrument, std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries>> current_market_data_snapshot;
        
    for ( int incremental_update_index = 0; incremental_update_index<incrementalRefresh.c_NoMDEntries().size(); ++incremental_update_index )
    {
        auto mdEntry = incrementalRefresh.c_NoMDEntries()[incremental_update_index];
            
        auto securityExchange = mdEntry.SecurityExchange();
        auto symbol = mdEntry.Symbol();
            
        Instrument instrument( securityExchange, symbol );
            
        auto mdEntryList = current_market_data_snapshot.find( instrument );
            
        if ( mdEntryList == current_market_data_snapshot.end() )
        {
            std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries> entryList;
            entryList.push_back( mdEntry );
            current_market_data_snapshot.emplace( instrument, entryList );
        } else {
            mdEntryList->second.push_back(mdEntry);
        }
    }
    
        
    for ( auto& mdEntry : current_market_data_snapshot )
    {
        (*_incrementalRefreshMapPtr)[mdEntry.first] = mdEntry.second;
    }
}


} /* namespace FIXGateway */

