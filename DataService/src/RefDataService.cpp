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

#include "RefDataService.h"
#include <quickfix/FixValues.h>

#include <Common.h>
#include <SecurityList.hpp>
#include <SecurityListLogger.hpp>
#include <SecurityListPubSubTypes.hpp>
#include <SecurityListRequestPubSubTypes.hpp>
#include "SecurityListRequestDataReaderListenerImpl.hpp"

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

#include <thread>
#include <chrono>


namespace DistributedATS {


RefDataService::RefDataService(
            std::shared_ptr<distributed_ats_utils::basic_domain_participant> basic_domain_participant_ptr,
        const FIX::DatabaseConnectionID& dbConnectionID)
{
    _basic_domain_participant_ptr = basic_domain_participant_ptr;
    m_sqliteConnection = std::make_shared<DistributedATS::SQLiteConnection>(dbConnectionID);
    
    m_pInstrumentMapPtr = std::make_shared< InstrumentMap >();
    m_pUserInstruments = std::make_shared< UserInstrumentList >();
    
    
    _security_list_queue_ptr = std::make_shared<SecurityListRequestQueue>();
        
    initialize();
    
};
    
    
RefDataService::~RefDataService() {
    
    std::atomic_init(&_is_running, false);
    _service_thread.join();
};
    
    
void RefDataService::initialize()
{
    try
    {
        populateUserGroupInstrumentMap();
    } catch ( ... )
    {
        LOG4CXX_ERROR(logger, "Exception when populating Reference Data");
    }
};

void RefDataService::populateUserGroupInstrumentMap()
{
    DistributedATS::SQLiteQuery sqliteQuery("select i.instrument_name, i.properties, u.user_name, m.market_name from "
                               " user_code u, " \
                               " instrument i, " \
                               " market m, " \
                               " instrument_market_map im_map, " \
                               " user_group_market_map ugm_map " \
                               " where im_map.instrument_name=i.instrument_name and " \
                               " u.user_group=ugm_map.user_group and " \
                               " m.market_name=im_map.market_name and " \
                               " im_map.market_name=ugm_map.market_name");

    LOG4CXX_INFO(logger, "Populating security list");
        
    m_sqliteConnection->execute(sqliteQuery);
        
    for ( int instrument_index =0; instrument_index<sqliteQuery.rows(); instrument_index++)
    {
        std::string symbol = sqliteQuery.getValue(instrument_index,0);
        std::string properties = sqliteQuery.getValue(instrument_index,1);
        std::string username = sqliteQuery.getValue(instrument_index,2);
        std::string market = sqliteQuery.getValue(instrument_index,3);
        
        auto instrumentPtr = m_pInstrumentMapPtr->find(symbol);
        
        if ( instrumentPtr == m_pInstrumentMapPtr->end() )
        {
            instrumentPtr = m_pInstrumentMapPtr->emplace( symbol,
                                                         std::make_shared<Instrument>( market, symbol, properties ) ).first;
        }
            
        auto instrumentList = m_pUserInstruments->find( username );
            
        if ( instrumentList == m_pUserInstruments->end() )
        {
            instrumentList = m_pUserInstruments->emplace( username, std::make_shared< InstrumentList > ( InstrumentList() )).first;
        }
        
        instrumentList->second->emplace_back(instrumentPtr->second);
    }
}



void RefDataService::createSecurityListRequestListener()
{
    _security_list_request_topic_tuple = _basic_domain_participant_ptr->make_topic
     <  DistributedATS_SecurityListRequest::SecurityListRequestPubSubType,
     DistributedATS_SecurityListRequest::SecurityListRequest>
     ( SECURITY_LIST_REQUEST_TOPIC_NAME );
     
     _security_list_request_data_reader_tuple = _basic_domain_participant_ptr->make_data_reader_tuple(_security_list_request_topic_tuple,
                                            new SecurityListRequestDataReaderListenerImpl( _security_list_queue_ptr ),"SECURITY_LIST_REQUEST_FILTER", "DATS_DestinationUser = %0",{_basic_domain_participant_ptr->get_participant_name()});

    std::atomic_init(&_is_running, true);

    _service_thread = std::thread(&RefDataService::service, this);
}

void RefDataService::createSecurityListDataWriter()
{
    _security_list_topic_tuple = _basic_domain_participant_ptr->make_topic
        < DistributedATS_SecurityList::SecurityListPubSubType,
    DistributedATS_SecurityList::SecurityList>
            ( SECURITY_LIST_TOPIC_NAME );
    
    _security_list_dw = _basic_domain_participant_ptr->make_data_writer( _security_list_topic_tuple );
}
    
    
bool RefDataService::processRefDataRequest( const SecurityListRequestPtr& securityListRequestPtr )
{
    LOG4CXX_INFO(logger, "Received Security List Request : "
                 << ":" << securityListRequestPtr->DATS_Source()
                 << ":" << securityListRequestPtr->DATS_Destination() << ":" << securityListRequestPtr->SecurityReqID());
    
    
    auto instrumentListPtr = std::make_shared<DistributedATS::InstrumentList> ( DistributedATS::InstrumentList() );
    
    std::string username = securityListRequestPtr->DATS_SourceUser();
    
    if ( username.empty() )
        username = securityListRequestPtr->DATS_Source();
    
    populateUserInstrumentList( username, instrumentListPtr );
    
    DistributedATS_SecurityList::SecurityList securityList;
    
    securityList.fix_header().MsgType(FIX::MsgType_SecurityList);
    securityList.DATS_Source(securityListRequestPtr->DATS_Destination());
    securityList.DATS_Destination(securityListRequestPtr->DATS_Source());
    securityList.DATS_DestinationUser(securityListRequestPtr->DATS_SourceUser());
    
    securityList.SecurityReqID(securityListRequestPtr->SecurityReqID());
    securityList.SecurityResponseID("1");
    securityList.SecurityRequestResult(0);

    securityList.c_NoRelatedSym().resize( instrumentListPtr->size() );
    
    LOG4CXX_INFO(logger, "SecurityList Target : [" <<  securityList.DATS_Destination() << ":" <<
                 securityList.DATS_Source()  << "]");
    
    
    int instrument_index = 0;
    for ( const auto& instrument : *instrumentListPtr )
    {
        std::cout << instrument->symbol.c_str() << std::endl;
        
        securityList.c_NoRelatedSym()[ instrument_index ].Symbol(instrument->symbol);
        securityList.c_NoRelatedSym()[ instrument_index ].SecurityExchange (instrument->marketName);
        securityList.c_NoRelatedSym()[ instrument_index ].Text( instrument->properties );
        
        instrument_index++;
    };
    
    std::stringstream ss;
    SecurityListLogger::log(ss, securityList);
    LOG4CXX_INFO(logger, "Publishing SecurityList : [" <<  ss.str() << "]");
        
    int ret = _security_list_dw->write(&securityList);
    
    if (!ret) {
        LOG4CXX_ERROR(logger, "Security List write returned :" << ret);
    }

    return true;
}


int RefDataService::service (void)
{
	while(_is_running)
	{
        SecurityListRequestPtr security_list_ptr;
        
        if (_security_list_queue_ptr->pop(security_list_ptr))
        {
            processRefDataRequest(security_list_ptr);
        }
        
        std::this_thread::sleep_for(std::chrono::duration<long double, std::milli>(1000));

	};

	return 0;
}



} /* namespace DistributedATS */
