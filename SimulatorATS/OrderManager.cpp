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

#include "OrderManager.hpp"

#include <quickfix/FixValues.h>


using namespace SimulatorATS;


void OrderManager::insertNewOrder( const FIX::SessionID& sessionId,
                                          const Instrument& instrument,
                                          const OrderId& clOrdId,
                                          char side, int price, int quantity )
{
    FIX::Locker lock(_mutex);

    auto sessionInstrumentOrderBookPtr = _sessionInstrumentOrderBookPtr->find( sessionId );
  
    if ( sessionInstrumentOrderBookPtr == _sessionInstrumentOrderBookPtr->end() )
    {
        sessionInstrumentOrderBookPtr = ( _sessionInstrumentOrderBookPtr->emplace(sessionId, std::make_shared<InstrumentOrderBook>()) ).first;
    }
        
    auto instrumentOrderBookPtr = sessionInstrumentOrderBookPtr->second->find( instrument );
    
    if ( instrumentOrderBookPtr == sessionInstrumentOrderBookPtr->second->end() )
    {
        instrumentOrderBookPtr = ( sessionInstrumentOrderBookPtr->second->emplace( instrument, std::make_shared<OrderBook>()) ).first;
    }
        
    if ( instrumentOrderBookPtr->second->find( clOrdId ) == instrumentOrderBookPtr->second->end() )
    {
        auto orderPtr = std::make_shared<Order>();
        orderPtr->price = price;
        orderPtr->side = side;
        orderPtr->quantity = quantity;
        orderPtr->order_status = 0;
        
        instrumentOrderBookPtr->second->emplace(clOrdId, orderPtr);
    };
};


bool OrderManager::updateOrder( const FIX::SessionID& sessionID,
                 const Instrument& instrument, const OrderId& clOrdId, int cumQty,int avgPx, char execType
                 )
{
    FIX::Locker lock(_mutex);
        
    if ( execType == FIX::ExecType_NEW )
    {
        std::cout << "New Order ID: "  << sessionID << "|" << instrument << "|" << clOrdId
            << "|CumQty : " << cumQty << "|AvgPx : " << avgPx << "|OrderStatus : " << execType << std::endl;
    } else if ( execType == FIX::ExecType_PARTIAL_FILL )
    {
        std::cout << "Partial Fill: " << sessionID << "|" << instrument << "|" << clOrdId
            << "|CumQty : " << cumQty << "|AvgPx : " << avgPx << "|OrderStatus : " << execType << std::endl;
    } else if ( execType == FIX::ExecType_CANCELED )
    {
        std::cout << "Order Cancel : " << sessionID << "|" << instrument << "|" << clOrdId
            << "|CumQty : " << cumQty << "|AvgPx : " << avgPx << "|OrderStatus : " << execType << std::endl;
    }

    auto sessionOrderBookPtr = _sessionInstrumentOrderBookPtr->find( sessionID );
    
    if ( sessionOrderBookPtr == _sessionInstrumentOrderBookPtr->end() )
        return false;
    
    auto instrumentOrderBookPtr = sessionOrderBookPtr->second->find( instrument );
    
    if ( instrumentOrderBookPtr == sessionOrderBookPtr->second->end() )
        return false;

    auto orderPtr = instrumentOrderBookPtr->second->find( clOrdId );
      
    if ( orderPtr == instrumentOrderBookPtr->second->end() )
        return false;
    
    orderPtr->second->vwap = avgPx;
    orderPtr->second->filled_quantity = cumQty;
    orderPtr->second->order_status = execType;

    return true;
};

SessionInstrumentOrderBookPtr OrderManager::getPartiallyFilledOrder()
{
    FIX::Locker lock(_mutex);

    SessionInstrumentOrderBookPtr partiallyFilledOrders = std::make_shared<SessionInstrumentOrderBook>();
    
    for ( auto& sessionInstrumentOrderBooks : *_sessionInstrumentOrderBookPtr )
    {
        for ( auto& instrumentOrderBooks : *sessionInstrumentOrderBooks.second )
        {
            for ( auto& orders : *instrumentOrderBooks.second )
            {
                if ( orders.second->filled_quantity != 0 && orders.second->quantity>orders.second->filled_quantity )
                {
                    auto sessionInstrumentOrderBookPtr = partiallyFilledOrders->find( sessionInstrumentOrderBooks.first );
                    
                    if ( sessionInstrumentOrderBookPtr == partiallyFilledOrders->end() )
                    {
                        sessionInstrumentOrderBookPtr = ( partiallyFilledOrders->emplace(
                                                        sessionInstrumentOrderBooks.first, std::make_shared<InstrumentOrderBook>() ) ).first;
                    }
                    
                    auto instrumentOrderBookPtr = sessionInstrumentOrderBookPtr->second->find( instrumentOrderBooks.first );
                    
                    if ( instrumentOrderBookPtr == sessionInstrumentOrderBookPtr->second->end() )
                    {
                        instrumentOrderBookPtr = ( sessionInstrumentOrderBookPtr->second->emplace( instrumentOrderBooks.first, std::make_shared<OrderBook>()) ).first;
                    }
                    
                    instrumentOrderBookPtr->second->emplace( orders.first, orders.second );                    
                }
            };
        }
    }
    
    return partiallyFilledOrders;
};




