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

#include "RefDataService.h"

#include <SecurityListRequestTypeSupportImpl.h>
#include <SecurityListTypeSupportImpl.h>
#include "SecurityListRequestDataReaderListenerImpl.hpp"


#include <quickfix/FixValues.h>


#include <Common.h>

namespace DistributedATS {


RefDataService::RefDataService(std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
                   const FIX::DatabaseConnectionID& dbConnectionID,
				   ACE_Thread_Manager *thr_mgr ) :  ACE_Task<ACE_MT_SYNCH> (thr_mgr)
{
    m_basicDomainParticipantPtr = basicDomainParticipantPtr;
    m_sqliteConnection = std::make_shared<DistributedATS::SQLiteConnection>(dbConnectionID);
    
    m_pInstrumentMapPtr = std::make_shared< InstrumentMap >();
    m_pUserInstruments = std::make_shared< UserInstrumentList >();
        
    initialize();
};
    
    
RefDataService::~RefDataService() {
};
    
    
void RefDataService::initialize()
{
    try
    {
        //populateInstrumentIdToRefDataMap();
        populateUserGroupInstrumentMap();
    } catch ( ... )
    {
        ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) MAJOR: Exception when populating Reference Data\n")));
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

    ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t|%D) Populating security list\n")));
    
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
            // find should always find something for a given instrument id because of outer join when populating m_instrumentIdToRefDataMap
           // instrumentPtr->second->ref_data = m_instrumentIdToRefDataMap.find(symbol)->second;
        }
            
        auto instrumentList = m_pUserInstruments->find( username );
            
        if ( instrumentList == m_pUserInstruments->end() )
        {
            instrumentList = m_pUserInstruments->emplace( username, std::make_shared< InstrumentList > ( InstrumentList() )).first;
        }
        
        instrumentList->second->emplace_back(instrumentPtr->second);
    }
}

/*
void RefDataService::populateInstrumentIdToRefDataMap()
{
    FIX::MySQLQuery mySQLQuery("select i.instrument_id, case when ird.ref_data is null then '' else ird.ref_data end from instrument as i LEFT OUTER JOIN instrument_ref_data ird ON i.instrument_id = ird.instrument_id");
    
    ACE_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t|%D) Populating instrument reference data\n")));
    
    m_pMySqlConnectionPtr->execute(mySQLQuery);
        
    for ( int instrument_index =0; instrument_index<mySQLQuery.rows(); instrument_index++)
    {
        int instrument_id = std::atoi( mySQLQuery.getValue(instrument_index,0) );
        std::string ref_data = mySQLQuery.getValue(instrument_index,1);
        
        std::cout << "Instrument ID : " << instrument_id << ":" << ref_data << std::endl;
        
        m_instrumentIdToRefDataMap.emplace( instrument_id, std::make_shared<std::string>(ref_data) );
    }
}*/


void RefDataService::createSecurityListRequestListener( const std::string& data_service_filter_expression )
{
    DDS::Topic_var security_list_request_topic = m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_SecurityListRequest::SecurityListRequestTypeSupport_var,
    DistributedATS_SecurityListRequest::SecurityListRequestTypeSupportImpl >
            ( SECURITY_LIST_REQUEST_TOPIC_NAME );
    
    
    DDS::ContentFilteredTopic_ptr cft_security_list_request =
    m_basicDomainParticipantPtr->getDomainParticipant()->create_contentfilteredtopic( "SECURITY_LIST_REQUEST_FILTER", security_list_request_topic,
                                                                        data_service_filter_expression.c_str(),
                                                                        DDS::StringSeq());
    
    DDS::DataReaderListener_var securityListRequestDataListener(
                                                                new SecurityListRequestDataReaderListenerImpl( msg_queue() ) );
    
    m_basicDomainParticipantPtr->createDataReaderListener ( cft_security_list_request, securityListRequestDataListener );
}

void RefDataService::createSecurityListDataWriter()
{
    
    DDS::Topic_var security_list_topic = m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_SecurityList::SecurityListTypeSupport_var,
        DistributedATS_SecurityList::SecurityListTypeSupportImpl >
            ( SECURITY_LIST_TOPIC_NAME );
    

    
    m_security_list_dw = m_basicDomainParticipantPtr->createDataWriter< DistributedATS_SecurityList::SecurityListDataWriter_var,
    DistributedATS_SecurityList::SecurityListDataWriter > ( security_list_topic );
    
    DDS::DataReaderListener_var securityListRequestDataListener(
                                                                new SecurityListRequestDataReaderListenerImpl( msg_queue() ) );
    
}
    
    
bool RefDataService::processRefDataRequest( DistributedATS_SecurityListRequest::SecurityListRequest* securityListRequestPtr )
{

    std::cout << "Received Security List Request : "
                << " : " << securityListRequestPtr->m_Header.SenderCompID
				<< ":" << securityListRequestPtr->m_Header.TargetCompID << ":" << securityListRequestPtr->SecurityReqID << std::endl;
    
    
    auto instrumentListPtr = std::make_shared<DistributedATS::InstrumentList> ( DistributedATS::InstrumentList() );
    
    std::string username = securityListRequestPtr->m_Header.SenderSubID.in();
    
    if ( username.empty() )
        username = securityListRequestPtr->m_Header.SenderCompID;
    
    populateUserInstrumentList( username, instrumentListPtr );
    
    DistributedATS_SecurityList::SecurityList securityList;
    
    securityList.m_Header.MsgType = CORBA::string_dup(FIX::MsgType_SecurityList);
    securityList.m_Header.SenderCompID = securityListRequestPtr->m_Header.TargetCompID;
    securityList.m_Header.TargetCompID = securityListRequestPtr->m_Header.SenderCompID;
    
    securityList.m_Header.TargetSubID = securityListRequestPtr->m_Header.SenderSubID;
    
    securityList.SecurityReqID = securityListRequestPtr->SecurityReqID;
    securityList.SecurityResponseID = CORBA::string_dup("1");
    securityList.SecurityRequestResult = 0;
    
    securityList.c_NoRelatedSym.length( (int)instrumentListPtr->size() );
    
    std::cout << "Sending to : " << securityList.m_Header.SenderCompID << ":" <<  securityList.m_Header.TargetCompID << std::endl;
    
    int instrument_index = 0;
    for ( auto& instrument : *instrumentListPtr )
    {
        std::cout << instrument->symbol.c_str() << std::endl;
        
        securityList.c_NoRelatedSym[ instrument_index ].Symbol = CORBA::string_dup(instrument->symbol.c_str());
        securityList.c_NoRelatedSym[ instrument_index ].SecurityExchange = CORBA::string_dup(instrument->marketName.c_str());
        securityList.c_NoRelatedSym[ instrument_index ].Text = CORBA::string_dup( instrument->properties.c_str() );
        
        instrument_index++;
    };
    
    int ret = m_security_list_dw->write(securityList, NULL);
    
    if (ret != DDS::RETCODE_OK)
    {
        ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: Security List write returned %d.\n"), ret));
    }

    
    return true;
}


int RefDataService::svc (void)
{
	while(1)
	{
        ACE_Message_Block* messageBlock = 0;
        
        ACE_Time_Value interval (0, 1000);
        
        if ( this->getq (messageBlock, &interval) > -1 )
        {
            DistributedATS_SecurityListRequest::SecurityListRequest* securityListRequestPtr = (DistributedATS_SecurityListRequest::SecurityListRequest*)messageBlock->rd_ptr();
             processRefDataRequest(securityListRequestPtr);
             messageBlock->release();
        } else {
            ACE_OS::sleep(interval);
        }
	};

	return 0;
}



} /* namespace DistributedATS */
