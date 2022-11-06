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

#include <MarketDataSnapshotFullRefreshTypeSupportImpl.h>

#include "MarketDataService.h"
#include <iostream>

#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"
#include "MarketDataRequestDataReaderListenerImpl.h"

#include <quickfix/FixValues.h>


namespace DistributedATS {

MarketDataService::MarketDataService( std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
        const FIX::DatabaseConnectionID& dbConnectionID,
		ACE_Thread_Manager *thr_mgr) :
		ACE_Task <ACE_MT_SYNCH> (thr_mgr), m_basicDomainParticipantPtr( basicDomainParticipantPtr )
{
    
    _incrementalRefreshMapPtr = std::make_shared<IncrementalRefreshMap>();
    
    m_sqliteConnection.reset( new DistributedATS::SQLiteConnection(dbConnectionID) );

    initialize();

}

MarketDataService::~MarketDataService() {
	// TODO Auto-generated destructor stub
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
    
    ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t|%D) Populating hist stats\n")));

    m_sqliteConnection->execute(query);
    
    for ( int instrument_index =0; instrument_index<query.rows(); instrument_index++)
    {
        std::string symbol = query.getValue(instrument_index,0);
        std::string market = query.getValue(instrument_index,1);
        int last_trade_price = ACE_OS::atoi(query.getValue(instrument_index,2).c_str());

        std::cout << "Last traded Price : " << symbol << "|" << market << "|" << last_trade_price << std::endl;

        /*Instrument instrument(instrument_group, symbol);
        std::list<DATSMarketDataIncrementalRefresh::NoMDEntries> initial_list;*/

        DistributedATS_MarketDataIncrementalRefresh::NoMDEntries last_price_entry;
        last_price_entry.MDUpdateAction = FIX::MDUpdateAction_NEW;
        last_price_entry.MDEntryType = FIX::MDEntryType_OPENING_PRICE;
        last_price_entry.MDEntryPx = last_trade_price;

        _incrementalRefreshMapPtr->emplace(Instrument( market, symbol),
        		std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries> { last_price_entry } );


    }


}

void MarketDataService::createMarketDataRequestListener(  const std::string& data_service_filter_expression/*,
                                                            const char* market_data_request_topic_name*/ )
{
    DDS::Topic_var market_data_request_topic =  m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_MarketDataRequest::MarketDataRequestTypeSupport_var,
    DistributedATS_MarketDataRequest::MarketDataRequestTypeSupportImpl >
        ( MARKET_DATA_REQUEST_TOPIC_NAME );
    
    std::cout << "Market Data Request Filter :" << data_service_filter_expression.c_str() << std::endl;

    DDS::ContentFilteredTopic_ptr cft_market_data_request =
    m_basicDomainParticipantPtr->getDomainParticipant()->create_contentfilteredtopic( "MARKET_DATA_REQUEST_FILTER",
                                                                                     market_data_request_topic,
                                                                        data_service_filter_expression.c_str(),
                                                                        DDS::StringSeq());
    DDS::DataReaderListener_var marketDataRequestDataListener(
            new DistributedATS::MarketDataRequestDataReaderListenerImpl( msg_queue() )
    );

    m_basicDomainParticipantPtr->createDataReaderListener( cft_market_data_request, marketDataRequestDataListener );
    
}

void MarketDataService::createMarketDataIncrementalRefreshListener()
{
    DDS::Topic_var market_data_incremental_refresh_topic =  m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshTypeSupport_var,
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshTypeSupportImpl >
        ( MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME );
    
    DDS::DataReaderListener_var marketDataIncrementalRefreshtDataListener(new DistributedATS::MarketDataIncrementalRefreshDataReaderListenerImpl(_incrementalRefreshMapPtr));
    
    m_basicDomainParticipantPtr->createDataReaderListener( market_data_incremental_refresh_topic, marketDataIncrementalRefreshtDataListener );
}
    
void MarketDataService::createMarketDataFullRefreshDataWriter()
{
    DDS::Topic_var market_data_snapshot_full_refresh_topic =  m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefreshTypeSupport_var,
    DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefreshTypeSupportImpl >
        ( MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME );
    
   _market_data_shapshot_full_refresh_dw =
        m_basicDomainParticipantPtr->createDataWriter< DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefreshDataWriter_var,
    DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefreshDataWriter >
        ( market_data_snapshot_full_refresh_topic );
    
}
    
bool MarketDataService::processMarketDataRequest( DistributedATS_MarketDataRequest::MarketDataRequest* marketDataRequestPtr )
{
    std::cout << ">>>> Received MarketDataRequest : " << marketDataRequestPtr->m_Header.SenderCompID << ":" 
		<< marketDataRequestPtr->m_Header.TargetCompID 
		<< ":" << marketDataRequestPtr->m_Header.SenderSubID << std::endl;
    
    for ( int symbol_index = 0; symbol_index<marketDataRequestPtr->c_NoRelatedSym.length(); ++symbol_index )
    {
        std::cout << "Symbol to send full snapshot : " << marketDataRequestPtr->c_NoRelatedSym[symbol_index].Symbol.in() << std::endl;
        
        DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh marketDataSnapshotFullRefresh;
        
        marketDataSnapshotFullRefresh.m_Header.BeginString = marketDataRequestPtr->m_Header.BeginString;
        marketDataSnapshotFullRefresh.m_Header.TargetCompID = marketDataRequestPtr->m_Header.SenderCompID;
        marketDataSnapshotFullRefresh.m_Header.SenderCompID = marketDataRequestPtr->m_Header.TargetCompID;
        marketDataSnapshotFullRefresh.m_Header.TargetSubID = marketDataRequestPtr->m_Header.SenderSubID;
        marketDataSnapshotFullRefresh.m_Header.MsgType = CORBA::string_dup("W");
        
        if ( populateMarketDataSnapshotFullRefresh( DistributedATS::Instrument( 
			marketDataRequestPtr->c_NoRelatedSym[symbol_index].SecurityExchange.in(),
			marketDataRequestPtr->c_NoRelatedSym[symbol_index].Symbol.in() ), marketDataSnapshotFullRefresh ) )
        {
            std::cout << "Publishing Full Market Data Snapshot : " << marketDataSnapshotFullRefresh.Symbol << std::endl;
            int ret = _market_data_shapshot_full_refresh_dw->write(marketDataSnapshotFullRefresh, NULL);
            
            if (ret != DDS::RETCODE_OK)
            {
                ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: Market Data Snapshot Data Write returned %d.\n"), ret));
            }
        }
        
        
    }
   
    return true;
}

int MarketDataService::svc (void)
{
	while(1)
	{
		ACE_Message_Block* messageBlock = 0;
        
		ACE_Time_Value interval (0, 250000);
        
		if ( this->getq (messageBlock, &interval) > -1 )
		{
            DistributedATS_MarketDataRequest::MarketDataRequest* marketDataRequestPtr = (DistributedATS_MarketDataRequest::MarketDataRequest*)messageBlock->rd_ptr();
			processMarketDataRequest(marketDataRequestPtr);
			messageBlock->release();
		} else {
			ACE_OS::sleep(interval);
		}
	}
}

bool MarketDataService::populateMarketDataSnapshotFullRefresh( const Instrument& instrument,
                                                              DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& marketDataSnapshotFullRefresh ) const
{
	marketDataSnapshotFullRefresh.Symbol = CORBA::string_dup( instrument.symbol.c_str() );
	marketDataSnapshotFullRefresh.SecurityExchange = CORBA::string_dup( instrument.marketName.c_str() );

	auto mdEntryList = _incrementalRefreshMapPtr->find( instrument );

	if ( mdEntryList == _incrementalRefreshMapPtr->end() )
		return false;

	marketDataSnapshotFullRefresh.c_NoMDEntries.length( (int)mdEntryList->second.size());

	auto md_entry_index = 0;
	for ( const auto& mdEntry : mdEntryList->second )
	{
		marketDataSnapshotFullRefresh.c_NoMDEntries[md_entry_index].MDEntryType = mdEntry.MDEntryType;
		marketDataSnapshotFullRefresh.c_NoMDEntries[md_entry_index].MDEntryPx = mdEntry.MDEntryPx;
		marketDataSnapshotFullRefresh.c_NoMDEntries[md_entry_index].MDEntrySize = mdEntry.MDEntrySize;

		md_entry_index++;
	}

	return true;
}


} /* namespace DistributedATS */
