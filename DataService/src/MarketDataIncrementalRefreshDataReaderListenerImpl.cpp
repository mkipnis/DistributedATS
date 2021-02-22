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
#include <quickfix/fix50/MarketDataIncrementalRefresh.h>
#include <MarketDataIncrementalRefreshLogger.hpp>

namespace DistributedATS {

MarketDataIncrementalRefreshDataReaderListenerImpl::~MarketDataIncrementalRefreshDataReaderListenerImpl() {
	// TODO Auto-generated destructor stub
}


void MarketDataIncrementalRefreshDataReaderListenerImpl::on_data_available( DDS::DataReader_ptr reader) throw (CORBA::SystemException)
{
    try
    {
        DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshDataReader_var
			market_data_incremental_refresh_dr = DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshDataReader::_narrow(reader);

        if (CORBA::is_nil ( market_data_incremental_refresh_dr.in() ) )
        {
            ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) MarketDataIncrementalRefreshDataReaderListenerImpl::on_data_available: _narrow failed.\n")));

            ACE_OS::exit(1);
        }

        while( true )
        {
            DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh marketDataRefresh;
            DDS::SampleInfo si ;
            DDS::ReturnCode_t status = market_data_incremental_refresh_dr->take_next_sample( marketDataRefresh, si );

            if (status == DDS::RETCODE_OK)
            {
                if ( !si.valid_data )
                    continue;
                
                std::stringstream ss;
                MarketDataIncrementalRefreshLogger::log(ss, marketDataRefresh);
                ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D)  MarketDataIncrementalRefresh : %s\n"), ss.str().c_str()));

                insertIncrementalMarketDataRefresh(marketDataRefresh);
                
            } else if (status == DDS::RETCODE_NO_DATA) {
                break;
            } else {
                ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) Read DATS::MarketDataIncrementalRefresh: %d.\n"), status));
            }
        }

    } catch (CORBA::Exception& e) {
        
        std::stringstream ss;
        ss << "Exception caught in read:" << std::endl << e << std::endl;
        ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) MarketDataIncrementalRefresh %s.\n"), ss.str().c_str()));

        ACE_OS::exit(1);
    }
}
    
    
    
    void MarketDataIncrementalRefreshDataReaderListenerImpl::insertIncrementalMarketDataRefresh( const DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& incrementalRefresh )
    {
        std::map<Instrument, std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries>> current_market_data_snapshot;
        
        for ( int incremental_update_index = 0; incremental_update_index<incrementalRefresh.c_NoMDEntries.length(); ++incremental_update_index )
        {
            DistributedATS_MarketDataIncrementalRefresh::NoMDEntries mdEntry = incrementalRefresh.c_NoMDEntries[incremental_update_index];
            
            std::string securityExchange = mdEntry.SecurityExchange.in();
            std::string symbol = mdEntry.Symbol.in();
            
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

