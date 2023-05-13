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

#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>
#include <utility>


namespace SimulatorATS
{
    

void Application::SecurityListRequest( const FIX::SessionID& sessionID, const FIX::SecurityReqID& secListRequestID )
{
    std::cout << "Sending Security List Request for Session : " << sessionID << " : Request ID : " << secListRequestID << std::endl;
	FIX44::SecurityListRequest securityListRequest(secListRequestID, FIX::SecurityListRequestType_ALL_SECURITIES);

	FIX::Header& header = securityListRequest.getHeader();

	FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
	FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
	header.setField(senderCompID);
	header.setField(targetCompID);

	FIX::Session::sendToTarget(securityListRequest);
}

void Application::OrderMassStatusRequest(const FIX::SessionID& sessionID, const FIX::MassStatusReqID& massStatusReqID )
{
    std::cout << "Sending Order Mass Status Request for Session : " << sessionID << " : Request ID : " << massStatusReqID << std::endl;
    FIX44::OrderMassStatusRequest orderMassStatusRequest(massStatusReqID, FIX::MassStatusReqType_STATUS_FOR_ALL_ORDERS);

	FIX::Header& header = orderMassStatusRequest.getHeader();

	FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
	FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
	header.setField(senderCompID);
	header.setField(targetCompID);

	FIX::Session::sendToTarget(orderMassStatusRequest);
	std::cout << "Done Sending Mass Status Request" << std::endl;
}

//
//  Send market data snapshot request, upon receiving security list
//
void Application::onMessage(const FIX44::SecurityList& securityList, const FIX::SessionID& sessionID)
{
    std::cout << "Recieved Security List for session : " << sessionID << ":" <<
    securityList << std::endl;
	FIX::NoRelatedSym numberOfSymbols;
	securityList.getField(numberOfSymbols);

	FIX::MDReqID mdReqID("MarketDataRequest1");
	FIX::SubscriptionRequestType subscriptionType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES);
	FIX::MarketDepth marketDepth(0);

	FIX44::MarketDataRequest marketDataRequest(mdReqID, subscriptionType, marketDepth);

	FIX44::MarketDataRequest::NoMDEntryTypes noMDEntryTypeBid;
	FIX::MDEntryType entryTypeBid(FIX::MDEntryType_BID);
	noMDEntryTypeBid.set(entryTypeBid);
	marketDataRequest.addGroup(noMDEntryTypeBid);

	FIX44::MarketDataRequest::NoMDEntryTypes noMDEntryTypeAsk;
	FIX::MDEntryType entryTypeAsk(FIX::MDEntryType_OFFER);
	noMDEntryTypeAsk.set(entryTypeAsk);
	marketDataRequest.addGroup(noMDEntryTypeAsk);

	for (int instrument_index = 0; instrument_index < numberOfSymbols; ++instrument_index)
	{
		auto noRelatedSymbolGroup = securityList.getGroupRef(instrument_index+1, FIX::FIELD::NoRelatedSym);

		FIX::Symbol symbol;
		noRelatedSymbolGroup.getField(symbol);

		FIX::SecurityExchange securityExchange;
		noRelatedSymbolGroup.getField( securityExchange );

		FIX44::MarketDataRequest::NoRelatedSym marketDataRequestRelatedSym;
		marketDataRequestRelatedSym.set(symbol);
        marketDataRequestRelatedSym.set(securityExchange);
        
		marketDataRequest.addGroup(marketDataRequestRelatedSym);
	}

	FIX::Header& header = marketDataRequest.getHeader();

	FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
	FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
	header.setField(senderCompID);
	header.setField(targetCompID);

	FIX::Session::sendToTarget(marketDataRequest);
}

void Application::onLogon( const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "Logon Received Submitting Security List Request - " << sessionID << std::endl;
    
  _login_cv.notify_all();
}

void Application::onMessage(const FIX44::MarketDataIncrementalRefresh& marketDataIncrementalRefresh, const FIX::SessionID& sessionID )
{
	FIX::NoMDEntries noMDEntries;

	marketDataIncrementalRefresh.getField(noMDEntries);

    SimulatorATS::InstrumentMarketDataPtr instrumentMarketDataPtr = std::make_shared<InstrumentMarketData>();

	for (auto marketDataEntry = 0; marketDataEntry < noMDEntries; ++marketDataEntry)
	{
		auto marketDataGroup = marketDataIncrementalRefresh.getGroupRef(marketDataEntry+1, FIX::FIELD::NoMDEntries);

		FIX::Symbol symbol;
		marketDataGroup.getField(symbol);

		FIX::SecurityExchange securityExchange;
		marketDataGroup.getField(securityExchange);

		Instrument instrument( securityExchange.getValue().c_str(), symbol.getValue().c_str() );

		FIX::MDEntryType mdEntryType;
		marketDataGroup.getField(mdEntryType);

		FIX::MDEntryPx mdEntryPx;
		marketDataGroup.getField(mdEntryPx);

		FIX::MDEntrySize mdEntrySize;
		marketDataGroup.getField(mdEntrySize);

		auto marketDataEntryPtr = instrumentMarketDataPtr->find( instrument );

		if ( marketDataEntryPtr == instrumentMarketDataPtr->end() )
            marketDataEntryPtr = ( instrumentMarketDataPtr->emplace( instrument, std::make_shared<SimulatorATS::MarketDataEntry>() ) ).first;
        
        SimulatorATS::PriceLevelPtr priceLevelPtr = std::make_shared<PriceLevel>();
        priceLevelPtr->emplace( mdEntryPx.getValue(), mdEntrySize.getValue() );
        
        auto marketDataEntryEntity = marketDataEntryPtr->second->find( mdEntryType );
        
        if ( marketDataEntryEntity == marketDataEntryPtr->second->end() )
            marketDataEntryEntity = ( marketDataEntryPtr->second->emplace( mdEntryType, std::make_shared< MarketDataEntryElement >() ) ).first;
        
        marketDataEntryEntity->second->push_back( priceLevelPtr );

    }

	for ( auto& instrumentMarketData : *instrumentMarketDataPtr )
        m_marketPtr->insertMarketDataEntry( sessionID, instrumentMarketData.first, instrumentMarketData.second );
}

void Application::onLogout( const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
  const FIX::Header& header = message.getHeader();
 // const FIX::MsgType& msgType = FIELD_GET_REF(header, MsgType);
  crack( message, sessionID );
  //std::cout << std::endl << "IN: " << message << std::endl;
}

void Application::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
  try
  {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField( possDupFlag );
    if ( possDupFlag ) throw FIX::DoNotSend();
  }
  catch ( FIX::FieldNotFound& ) {}

}

void Application::toAdmin( FIX::Message& message, const FIX::SessionID& sessionID )
{
  const FIX::Header& header = message.getHeader();

  const FIX::MsgType& msgType = FIELD_GET_REF( header, MsgType );

  if ( msgType == FIX::MsgType_Logon )
  {
      auto password = m_sessionSettings.get(sessionID).getString("Password");
      
      FIX::Username userName( sessionID.getSenderCompID() );
      message.setField(userName);

      FIX::Password fixPassword(password);
      message.setField(fixPassword);

    //std::cout << std::endl << "OUT: " << message << std::endl;
  }
}


void Application::onMessage( const FIX44::OrderCancelReject& message , const FIX::SessionID& sessionID )
{
	std::cout << "Cancel Reject : " << sessionID << ":" << message << std::endl;
}

void Application::onMessage( const FIX44::OrderMassCancelReport& message, const  FIX::SessionID& sessionID )
{
    std::cout << "Order Mass Cancel Report : " << sessionID << ":" << message << std::endl;
}

void Application::onMessage(const FIX44::ExecutionReport& message, const FIX::SessionID& sessionID)
{
    FIX::CumQty cumQty;
    message.getField(cumQty);
    
    FIX::ExecID execId;
    message.getField(execId);

    FIX::OrderID orderId;
    message.getField(orderId);
    
    FIX::AvgPx avgPx;
    message.getField(avgPx);
    
    FIX::ExecType execType;
    message.getField(execType);
    
    FIX::OrderID fixOrderId;
    message.getField(fixOrderId);

    FIX::Symbol symbol;
    message.getField(symbol);
              
    FIX::SecurityExchange securityExchange;
    message.getField(securityExchange);
    
    Instrument instrument( securityExchange.getValue().c_str(), symbol.getValue().c_str() );
    
    m_orderManagerPtr->updateOrder( sessionID, instrument, orderId, cumQty, avgPx, execType );

}

void Application::onMessage( const FIX44::MarketDataSnapshotFullRefresh& marketDataSnapshotFullRefresh, const FIX::SessionID& sessionID)
{
    FIX::NoMDEntries noMDEntries;
    
    marketDataSnapshotFullRefresh.getField(noMDEntries);
    
    //typedef std::map<int, int> price_size_map;
    //typedef std::map<std::string, price_size_map> side_price_map;
    
    FIX::Symbol symbol;
    marketDataSnapshotFullRefresh.getField(symbol);
    
    FIX::SecurityExchange securityExchange;
    marketDataSnapshotFullRefresh.getField(securityExchange);
    
    Instrument instrument( securityExchange.getValue().c_str(), symbol.getValue().c_str() );
    
    int worst_bid, worst_ask, open_price;
    int total_bid_quantity, total_ask_quantity;
    
    worst_bid = worst_ask = total_bid_quantity = total_ask_quantity = open_price = 0;
    
    SimulatorATS::MarketDataEntryPtr marketDataEntryPtr = std::make_shared<MarketDataEntry>();
        
    for (auto marketDataEntry = 0; marketDataEntry < noMDEntries; ++marketDataEntry)
    {
        auto marketDataGroup = marketDataSnapshotFullRefresh.getGroupRef(marketDataEntry+1, FIX::FIELD::NoMDEntries);
        
        FIX::MDEntryType mdEntryType;
        marketDataGroup.getField(mdEntryType);
        
        FIX::MDEntryPx mdEntryPx;
        marketDataGroup.getField(mdEntryPx);
        
        FIX::MDEntrySize mdEntrySize;
        marketDataGroup.getField(mdEntrySize);
        
        SimulatorATS::PriceLevelPtr priceLevelPtr = std::make_shared<PriceLevel>();
        priceLevelPtr->emplace( mdEntryPx, mdEntrySize);
                
        auto marketDataEntryEntity = marketDataEntryPtr->find( mdEntryType );
                
        if ( marketDataEntryEntity == marketDataEntryPtr->end() )
            marketDataEntryEntity = ( marketDataEntryPtr->emplace( mdEntryType, std::make_shared< MarketDataEntryElement >() ) ).first;
        
        marketDataEntryEntity->second->push_back( priceLevelPtr );
    }

    m_marketPtr->insertMarketDataEntry( sessionID, instrument, marketDataEntryPtr );
}


void Application::run()
{
    
    try
    {
        std::unique_lock<std::mutex> lk( _login_mutex );
        _login_cv.wait( lk );

        sleep(1); // this sleep is required when running everything on a single core box - FIXGateway might be still at the session reset state
        
        for ( auto& fixSession : FIX::Session::getSessions() )
        {
            FIX::SecurityReqID aSecurityReqID("SecurityListReq1");
            SecurityListRequest(fixSession, aSecurityReqID);
        }
        
        while ( true )
        {
            m_marketPtr->printMarketData();
                
            int operation = rand()%3;

            std::cout << "Running : " << operation << std::endl;

            switch ( operation )
            {
                // aggress on open orders and submit more orders
                case Operation::AGGRESS_ON_OPEN_ORDERS_AND_SUBMIT_MORE_ORDERS :
                    m_marketPtr->aggressOnBothSidesAndSubmitRandomOrders();
                break;
                // cancel partially filled orders
                case Operation::CANCEL_PARTIALLY_FILLED_ORDERS :
                {
                    auto partiallyFilledOrders = m_orderManagerPtr->getPartiallyFilledOrder();
                               
                    for ( auto& sessionInstrumentOrderBooks : *partiallyFilledOrders )
                        for ( auto& instrumentOrderBooks : *sessionInstrumentOrderBooks.second )
                            for ( auto& order : *instrumentOrderBooks.second )
                            {
                                m_marketPtr->cancelOrder( sessionInstrumentOrderBooks.first,
                                                             instrumentOrderBooks.first,
                                                             order.first,
                                                             order.second->side);
                                }

                    };
                    break;
                    // Mass cancel
                    case Operation::MASS_CANCEL:
                         for ( auto& fixSession : FIX::Session::getSessions() )
                            m_marketPtr->massCancelRequest(fixSession);
                    break;
                }
            
                sleep(1);
            }
        }
        catch ( std::exception & e )
        {
            std::cout << "Message Not Sent: " << e.what();
        }
    }
}
