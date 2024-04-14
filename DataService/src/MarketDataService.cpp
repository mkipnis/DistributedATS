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

#include "MarketDataService.h"
#include <BasicDomainParticipant.h>
#include <iostream>
#include <quickfix/FixValues.h>
#include <MarketDataRequestPubSubTypes.h>
#include <MarketDataIncrementalRefreshPubSubTypes.h>
#include <MarketDataSnapshotFullRefreshPubSubTypes.h>
#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

#include <thread>
#include <chrono>

#include <MarketDataSnapshotFullRefreshLogger.hpp>

#include <LoggerHelper.h>


namespace DistributedATS {

MarketDataService::MarketDataService( std::shared_ptr<distributed_ats_utils::basic_domain_participant> basic_domain_participant,
        const FIX::DatabaseConnectionID& dbConnectionID) :
    _basic_domain_participant_ptr( basic_domain_participant )
{
    
    _incrementalRefreshMapPtr = std::make_shared<IncrementalRefreshMap>();
    
    m_sqliteConnection.reset( new DistributedATS::SQLiteConnection(dbConnectionID) );
    
    _markat_data_request_queue_ptr = std::make_shared<MarketDataRequestQueue>();

    std::atomic_init(&_is_running, true);

    _service_thread = std::thread(&MarketDataService::service, this);
    
    initialize();

}

MarketDataService::~MarketDataService() {
    std::atomic_init(&_is_running, false);
    _service_thread.join();
}

void MarketDataService::initialize()
{

    DistributedATS::SQLiteQuery query("select i.instrument_name, m.market_name, json_extract(hp.properties,\"$.open\") as open_price " \
                               " from " \
                               " hist_price hp, " \
                               " instrument i, " \
                               " market m, " \
                               " instrument_market_map imm " \
                               " where hp.instrument_name=i.instrument_name and " \
                               " imm.instrument_name=i.instrument_name and " \
                               " m.market_name=imm.market_name and " \
                               " hp.business_date=(select max(business_date) from hist_price where instrument_name=i.instrument_name)");
    
    LOG4CXX_INFO(logger, "Populating hist stats");
    
    m_sqliteConnection->execute(query);
    
    for ( int instrument_index =0; instrument_index<query.rows(); instrument_index++)
    {
        std::string symbol = query.getValue(instrument_index,0);
        std::string market = query.getValue(instrument_index,1);
        int last_trade_price = std::atoi(query.getValue(instrument_index,2).c_str());

        std::cout << "Last traded Price : " << symbol << "|" << market << "|" << last_trade_price << std::endl;

        /*Instrument instrument(instrument_group, symbol);
        std::list<DATSMarketDataIncrementalRefresh::NoMDEntries> initial_list;*/

        DistributedATS_MarketDataIncrementalRefresh::NoMDEntries last_price_entry;
        last_price_entry.MDUpdateAction(FIX::MDUpdateAction_NEW);
        last_price_entry.MDEntryType(FIX::MDEntryType_OPENING_PRICE);
        last_price_entry.MDEntryPx(last_trade_price);
        last_price_entry.TimeInForce(1);

        _incrementalRefreshMapPtr->emplace(Instrument( market, symbol),
        		std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries> { last_price_entry } );

    }

}

void MarketDataService::createMarketDataRequestListener( const std::string& data_service_name )
{
    
    DistributedATS_MarketDataRequest::MarketDataRequest marketDataRequest;
    
    _market_data_request_topic_tuple = _basic_domain_participant_ptr->make_topic
        <
    DistributedATS_MarketDataRequest::MarketDataRequestPubSubType,
    DistributedATS_MarketDataRequest::MarketDataRequest>
        ( MARKET_DATA_REQUEST_TOPIC_NAME );
    
    
    _market_data_request_data_reader_tuple =  _basic_domain_participant_ptr->make_data_reader_tuple(_market_data_request_topic_tuple, new DistributedATS::MarketDataRequestDataReaderListenerImpl ( data_service_name, _markat_data_request_queue_ptr ) );
    
}

void MarketDataService::createMarketDataIncrementalRefreshListener()
{
    _market_data_incremental_refresh_topic_tuple =  _basic_domain_participant_ptr->make_topic
        < DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshPubSubType,
        DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>
        ( MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME );
    
    _market_data_incremental_refresh_data_reader_tuple =  _basic_domain_participant_ptr->make_data_reader_tuple(_market_data_incremental_refresh_topic_tuple,
            new DistributedATS::MarketDataIncrementalRefreshDataReaderListenerImpl(_incrementalRefreshMapPtr));
    
}
    
void MarketDataService::createMarketDataFullRefreshDataWriter()
{
    _market_data_snapshot_full_refresh_topic_tuple = _basic_domain_participant_ptr->make_topic
        < DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefreshPubSubType,
    DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh>
        ( MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME );
    

    
   _market_data_shapshot_full_refresh_dw =
    _basic_domain_participant_ptr->make_data_writer( _market_data_snapshot_full_refresh_topic_tuple );
    
}
    
bool MarketDataService::processMarketDataRequest( const MarketDataRequestPtr& marketDataRequestPtr )
{
    std::cout << ">>>> Received MarketDataRequest : " << marketDataRequestPtr->header().SenderCompID() << ":"
		<< marketDataRequestPtr->header().TargetCompID()
		<< ":" << marketDataRequestPtr->header().SenderSubID() << std::endl;
    
    for ( int symbol_index = 0; symbol_index<marketDataRequestPtr->c_NoRelatedSym().size(); ++symbol_index )
    {
        std::cout << "Symbol to send full snapshot : " << marketDataRequestPtr->c_NoRelatedSym()[symbol_index].Symbol() << std::endl;
        
        DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh marketDataSnapshotFullRefresh;
        
        marketDataSnapshotFullRefresh.header().BeginString(marketDataRequestPtr->header().BeginString());
        marketDataSnapshotFullRefresh.header().TargetCompID(marketDataRequestPtr->header().SenderCompID());
        marketDataSnapshotFullRefresh.header().SenderCompID(marketDataRequestPtr->header().TargetCompID());
        marketDataSnapshotFullRefresh.header().TargetSubID(marketDataRequestPtr->header().SenderSubID());
        marketDataSnapshotFullRefresh.header().MsgType("W");
        
        if ( populateMarketDataSnapshotFullRefresh( DistributedATS::Instrument( 
			marketDataRequestPtr->c_NoRelatedSym()[symbol_index].SecurityExchange(),
			marketDataRequestPtr->c_NoRelatedSym()[symbol_index].Symbol() ), marketDataSnapshotFullRefresh ) )
        {
            std::cout << "Publishing Full Market Data Snapshot : " << marketDataSnapshotFullRefresh.Symbol() << std::endl;
            int ret = _market_data_shapshot_full_refresh_dw->write(&marketDataSnapshotFullRefresh);
            
            if (!ret) {
                LOG4CXX_ERROR(logger, "Market Data Snapshot Data Write returned : " << ret );
            }

        }
        
        
    }
   
    return true;
}

int MarketDataService::service (void)
{
    
    while(_is_running)
    {

        std::shared_ptr<DistributedATS_MarketDataRequest::MarketDataRequest> market_data_request;
        
        if ( _markat_data_request_queue_ptr->pop( market_data_request ) )
        {
            processMarketDataRequest(market_data_request);
        } else {
            std::this_thread::sleep_for(std::chrono::duration<long double, std::milli>(1000));
        }
    };
}

bool MarketDataService::populateMarketDataSnapshotFullRefresh( const Instrument& instrument,
                                                              DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& marketDataSnapshotFullRefresh ) const
{
	marketDataSnapshotFullRefresh.Symbol( instrument.symbol );
	marketDataSnapshotFullRefresh.SecurityExchange( instrument.marketName );

	auto mdEntryList = _incrementalRefreshMapPtr->find( instrument );

	if ( mdEntryList == _incrementalRefreshMapPtr->end() )
		return false;

	marketDataSnapshotFullRefresh.c_NoMDEntries().resize( mdEntryList->second.size());

	auto md_entry_index = 0;
	for ( const auto& mdEntry : mdEntryList->second )
	{
        marketDataSnapshotFullRefresh.c_NoMDEntries()[md_entry_index].MDEntryType(mdEntry.MDEntryType());
		marketDataSnapshotFullRefresh.c_NoMDEntries()[md_entry_index].MDEntryPx(mdEntry.MDEntryPx());
		marketDataSnapshotFullRefresh.c_NoMDEntries()[md_entry_index].MDEntrySize(mdEntry.MDEntrySize());
        marketDataSnapshotFullRefresh.c_NoMDEntries()[md_entry_index].TimeInForce(0);

		md_entry_index++;
	}
    
    
    LoggerHelper::log_debug<
    std::stringstream, MarketDataSnapshotFullRefreshLogger,
    DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh>
    (logger, marketDataSnapshotFullRefresh, "MarketDataSnapshotFullRefresh");

	return true;
}



} /* namespace DistributedATS */
