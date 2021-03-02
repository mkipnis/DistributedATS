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

#ifndef REFDATASERVICE_H_
#define REFDATASERVICE_H_

#define HAVE_MYSQL

#include <SecurityListRequestTypeSupportImpl.h>
#include <SecurityListTypeSupportImpl.h>

#include <ace/Guard_T.h>
#include <ace/RW_Mutex.h>
#include <memory>

#include <vector>

#include <quickfix/Exceptions.h>
#include <quickfix/DatabaseConnectionID.h>
#include <quickfix/MySQLConnection.h>

#include <iostream>

#include <ace/Singleton.h>

#include <list>
#include <map>

#include <BasicDomainParticipant.h>


namespace DistributedATS {

struct Instrument
{
    Instrument() = default;
    Instrument(Instrument const&) = default;
    Instrument(Instrument&&) = default;
    Instrument& operator=(Instrument&) = default;
    Instrument& operator=(Instrument&&) = default;
    virtual ~Instrument() = default;
    
	Instrument( const char* marketNameIn, const char* symbolIn )
		: marketName( marketNameIn ),  symbol( symbolIn )
	{
        ref_data.reset();
	};

	Instrument( std::string& marketNameIn, std::string& symbolIn )
		: marketName( marketNameIn  ),  symbol( symbolIn )
	{
        ref_data.reset();
	};

    uint32_t instrument_id;
	std::string symbol;
	std::string marketName; // FIX SecurityExchange
    std::shared_ptr<std::string> ref_data;

	friend bool operator<( const Instrument& i1, const Instrument& i2 )
	{
		return ( i1.symbol + i1.marketName < i2.symbol + i2.marketName );
	};

	friend bool operator==( const Instrument& i1, const Instrument& i2 )
	{
		return ( i1.symbol == i2.symbol && i1.marketName ==  i2.marketName );
	};
};

typedef std::shared_ptr<Instrument> InstrumentPtr;
typedef std::map<uint32_t, InstrumentPtr> InstrumentMap;
typedef std::shared_ptr<InstrumentMap> InstrumentMapPtr;
typedef std::list<InstrumentPtr> InstrumentList;
typedef std::shared_ptr<InstrumentList> InstrumentListPtr;
typedef std::map<std::string, InstrumentListPtr> UserInstrumentList;
typedef std::shared_ptr<UserInstrumentList> UserInstrumentListPtr;

class RefDataService  : public ACE_Task <ACE_MT_SYNCH>
{

public:

    RefDataService( std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
                   const FIX::DatabaseConnectionID& dbConnectionID,
					ACE_Thread_Manager *thr_mgr);
    
    virtual ~RefDataService();


    void initialize();

    virtual int svc (void);
    
    void createSecurityListRequestListener( const std::string& data_service_filter_expression );
    void createSecurityListDataWriter();

    
    bool processRefDataRequest( DistributedATS_SecurityListRequest::SecurityListRequest* );

	const void populateUserInstrumentList( std::string& username, InstrumentListPtr& instrumentListPtrOut ) const
	{
		auto instrumentListPtr = m_pUserInstruments->find( username );	
		if ( instrumentListPtr != m_pUserInstruments->end() )
			instrumentListPtrOut->assign( instrumentListPtr->second->begin(),  instrumentListPtr->second->end() );
	};

protected:
    void populateUserGroupInstrumentMap();
    void populateInstrumentIdToRefDataMap();
    
private:

	std::shared_ptr<FIX::MySQLConnection> m_pMySqlConnectionPtr;
    std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> m_basicDomainParticipantPtr;
    
    // instrument_id to ref_data
    std::map<uint32_t,std::shared_ptr<std::string>> m_instrumentIdToRefDataMap;

	UserInstrumentListPtr m_pUserInstruments;
    
    InstrumentMapPtr m_pInstrumentMapPtr;
    
    DistributedATS_SecurityList::SecurityListDataWriter_var m_security_list_dw;
};

} /* namespace DistributedATS */

#endif /* AUTHSERVICESINGLETON_H_ */
