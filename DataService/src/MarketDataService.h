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

#pragma once

#include <map>
#include <list>
#include <memory>
#include <thread>

#include "MarketDataRequestDataReaderListenerImpl.h"

#include <MarketDataIncrementalRefresh.hpp>
#include <MarketDataSnapshotFullRefresh.hpp>

#include "RefDataService.h"
#include "SQLiteConnection.hpp"

#include <Common.h>


namespace DistributedATS {

    using IncrementalRefreshMap = std::map<Instrument, std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries>>;
    using IncrementalRefreshMapPtr = std::shared_ptr<IncrementalRefreshMap>;

class MarketDataService
{
public:
	MarketDataService( std::shared_ptr<distributed_ats_utils::basic_domain_participant>,
			 	 	 const FIX::DatabaseConnectionID& dbConnectionID);
	virtual ~MarketDataService();

	void initialize();
    
    void createMarketDataRequestListener();
    void createMarketDataIncrementalRefreshListener();
    void createMarketDataFullRefreshDataWriter();
    
    bool processMarketDataRequest( const MarketDataRequestPtr& marketDataRequestPtr );

	bool populateMarketDataSnapshotFullRefresh( const Instrument&,
                                               DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ) const;

    int service (void);

private:
    IncrementalRefreshMapPtr _incrementalRefreshMapPtr;
    
    std::shared_ptr<DistributedATS::SQLiteConnection> m_sqliteConnection;
    std::shared_ptr<distributed_ats_utils::basic_domain_participant> _basic_domain_participant_ptr;
    
    distributed_ats_utils::data_writer_ptr _market_data_shapshot_full_refresh_dw;
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_MarketDataRequest::MarketDataRequest> _market_data_request_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_MarketDataRequest::MarketDataRequest> _market_data_request_data_reader_tuple;
    // distributed_ats_utils::content_filtered_topic_ptr _cft_market_data_request;
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh> _market_data_incremental_refresh_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh> _market_data_incremental_refresh_data_reader_tuple;
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh> _market_data_snapshot_full_refresh_topic_tuple;
    
    //distributed_ats_utils::data_reader_listener_ptr _drk_market_data_incrementa_refresh;
    
    MarketDataRequestQueuePtr _markat_data_request_queue_ptr;
    
    //distributed_ats_utils::data_reader_listener_ptr _drl_market_data_request_ptr;
    
    std::thread _service_thread;
    
    std::atomic<bool> _is_running;
};


} /* namespace DistributedATS */
