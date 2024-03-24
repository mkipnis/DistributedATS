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


#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>

#include <algorithm>

using namespace LatencyTest;

uint32_t Application::_order_index = 0;

void Application::submitOrder(const FIX::SessionID& sessionID, 
		const FIX::Symbol& symbol,
		const FIX::SecurityExchange& securityExchange,
		const FIX::ClOrdID& clOrdID,
		const FIX::Side& side,
		const FIX::OrderQty& orderQty, 
		const FIX::Price& price
		)
{

	std::cout << "Lets submit an order" << std::endl;
	FIX::OrdType ordType;

	FIX44::NewOrderSingle newOrderSingle(clOrdID, side, FIX::TransactTime(), FIX::OrdType_LIMIT);

	newOrderSingle.set(FIX::HandlInst('1'));

	newOrderSingle.set(symbol);
	newOrderSingle.set(securityExchange);
	newOrderSingle.set(orderQty);
	newOrderSingle.set(FIX::TimeInForce(FIX::TimeInForce_GOOD_TILL_CANCEL));
	newOrderSingle.set(price);
    
    m_pLatencyStatsPtr->insertStats( m_last_order_id, OrderHopLatency::NEW_ORDER_SINGLE_FIX );
    
	std::cout << "Sending New Order Single : " <<  newOrderSingle << std::endl;

	FIX::Session::sendToTarget(newOrderSingle, sessionID);
}

void Application::cancelOrder(const FIX::SessionID& sessionID,
	const FIX::Symbol& symbol,
	const FIX::OrigClOrdID& origClOrdID,
	const FIX::ClOrdID& clOrdID,
	const FIX::Side& side
	)
{
	FIX44::OrderCancelRequest orderCancelRequest(origClOrdID, clOrdID, side, FIX::TransactTime());

	orderCancelRequest.set(symbol);

	FIX::Header& header = orderCancelRequest.getHeader();

	FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
	FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
	header.setField(senderCompID);
	header.setField(targetCompID);

	FIX::Session::sendToTarget(orderCancelRequest);
}


void Application::onLogon( const FIX::SessionID& sessionID )
{
    std::cout << std::endl << "Logon received lets start : " << sessionID << std::endl;
    _login_cv.notify_all();
}

void Application::onLogout( const FIX::SessionID& sessionID )
{
  std::cout << std::endl << "Logout - " << sessionID << std::endl;
  exit(0);
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
  crack( message, sessionID );
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
    std::cout << std::endl << "Sending Logon : " << sessionID << std::endl;

	FIX::Dictionary sessionSettingDictionary = m_sessionSettings.get(sessionID);

	std::string sessionPassword = sessionSettingDictionary.getString("Password");

    FIX::Username userName(sessionID.getSenderCompID());
    message.setField(userName);

    FIX::Password password(sessionPassword);
    message.setField(password);

  } else if (  msgType == FIX::MsgType_Logout )
  {
  	std::cout << "Logout : " <<  message << std::endl;
  }
}



void Application::onMessage
(const FIX44::ExecutionReport& message, const FIX::SessionID& sessionID )
{
	FIX::OrderID orderID;
	message.getField( orderID );

	std::cout << "Received Execution Report : " << orderID << std::endl;

	if (orderID.getValue() == m_last_order_id )
	{
		std::cout << "Recieved Execution Report for Order Id : " << orderID << std::endl;

		FIX::Symbol symbol;
		FIX::SecurityExchange securityExchange;
		FIX::Side side;
		FIX::Price price;
		FIX::OrderQty orderQty;

		m_last_order_id = getNextOrderId( symbol );
		FIX::ClOrdID clOrderID( m_last_order_id );
		message.get(securityExchange);
		message.get(symbol);
		message.get(side);
		message.get(price);
		message.get(orderQty);
        
        m_pLatencyStatsPtr->insertStats(orderID.getValue(), OrderHopLatency::EXECUTION_REPORT_FIX);
        
        if ( m_number_of_orders == _order_index )
        {
            std::cout << "Orders : " << m_number_of_orders << ":" << _order_index << std::endl;
            FIX::Session* session = FIX::Session::lookupSession(sessionID);
            session->logout("Done.");
            std::cout << "Stats:" << _min_latency << "|"<< _max_latency << "|" << _total_latency/m_number_of_orders << std::endl;
        } else {
            
            uint32_t gatewayNewOrderSingleLatency = 0;
            uint32_t matchingEngineLatency = 0;
            uint32_t gatewayExecutionReportLatency = 0;
            uint32_t roundTripLatency = 0;
            
            m_pLatencyStatsPtr->getLatencyStats(orderID.getValue(), gatewayNewOrderSingleLatency, matchingEngineLatency, gatewayExecutionReportLatency, roundTripLatency);
            
            std::cerr << "Round trip latency (Microseconds) |" << orderID << "|"
                    << gatewayNewOrderSingleLatency << "|"
                    << matchingEngineLatency << "|"
                    << gatewayExecutionReportLatency << "|"
                    << roundTripLatency << std::endl;
            
           // if ( _min_latency > roundTripLatency )
            _min_latency = std::min(_min_latency, roundTripLatency);
            _max_latency = std::max(_max_latency, roundTripLatency);
            
            _total_latency += roundTripLatency;
            
            
            submitOrder( sessionID, symbol, securityExchange ,  clOrderID, side, orderQty, price
                        );
        };
	}
}

void Application::onMessage(const FIX44::MarketDataIncrementalRefresh& marketDataIncrementalRefresh, const FIX::SessionID& sessionID )
{
};


void Application::run()
{
    try
    {
        std::unique_lock<std::mutex> lk( _login_mutex );
        _login_cv.wait( lk );

	sleep(1); // this sleep is required when running everything on a single core box - FIXGateway might be still at the session reset state 

        for ( auto& fixSession : FIX::Session::getSessions() )
        {
            FIX::Dictionary sessionSettingDictionary = m_sessionSettings.get(fixSession);

            std::string symbol = sessionSettingDictionary.getString("Symbol");
            std::string securityExchange = sessionSettingDictionary.getString("SecurityExchange");

            m_number_of_orders = sessionSettingDictionary.getInt("NumberOfOrders");
            std::cout << "Number of orders to send : " << m_number_of_orders << std::endl;

            m_last_order_id = getNextOrderId( symbol );

            FIX::ClOrdID clOrderID( m_last_order_id );
            FIX::Side side( FIX::Side_BUY );
            FIX::OrderQty orderQty( 10 );
            FIX::Price price( 10050 );

            submitOrder( fixSession, symbol, securityExchange, clOrderID, side, orderQty, price );
        }
    
        while ( true )
        {
            sleep(1000);
        }
        
    } catch ( std::exception & e )
    {
        std::cout << "Exception: " << e.what();
    }
}
