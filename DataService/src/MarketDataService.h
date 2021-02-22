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

#ifndef MARKETDATASERVICE_H_
#define MARKETDATASERVICE_H_

#include <map>
#include <list>
#include <memory>

#include <MarketDataIncrementalRefreshTypeSupportImpl.h>
#include <MarketDataSnapshotFullRefreshTypeSupportImpl.h>

#include "MarketDataRequestDataReaderListenerImpl.h"

#include "RefDataService.h"

#include <Common.h>



namespace DistributedATS {

    using IncrementalRefreshMap = std::map<Instrument, std::list<DistributedATS_MarketDataIncrementalRefresh::NoMDEntries>>;
    using IncrementalRefreshMapPtr = std::shared_ptr<IncrementalRefreshMap>;

class MarketDataService : public ACE_Task <ACE_MT_SYNCH>
{
public:
	MarketDataService( std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
			 	 	 const FIX::DatabaseConnectionID& dbConnectionID,
							ACE_Thread_Manager *thr_mgr);
	virtual ~MarketDataService();

	void initialize();
    
    void createMarketDataRequestListener( const std::string& data_service_filter_expression );
    void createMarketDataIncrementalRefreshListener();
    void createMarketDataFullRefreshDataWriter();
    
    bool processMarketDataRequest( DistributedATS_MarketDataRequest::MarketDataRequest* marketDataRequestPtr );

	bool populateMarketDataSnapshotFullRefresh( const Instrument&,
                                               DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ) const;

    virtual int svc (void);

private:
    IncrementalRefreshMapPtr _incrementalRefreshMapPtr;
    
	std::unique_ptr<FIX::MySQLConnection> m_pMySqlConnectionPtr;
    std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> m_basicDomainParticipantPtr;
    
    DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefreshDataWriter_var _market_data_shapshot_full_refresh_dw;
};


} /* namespace DistributedATS */

#endif /* MARKETDATASERVICE_H_ */
