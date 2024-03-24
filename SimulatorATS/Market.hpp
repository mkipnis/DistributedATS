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

#ifndef Market_hpp
#define Market_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <list>

#include <quickfix/SessionID.h>
#include <quickfix/Mutex.h>

#include "OrderManager.hpp"

#include "Instrument.hpp"

namespace SimulatorATS
{
    typedef std::map<int, int> PriceLevel;
    typedef std::shared_ptr<PriceLevel> PriceLevelPtr;
    
    typedef std::list< PriceLevelPtr > MarketDataEntryElement;
    typedef std::shared_ptr<MarketDataEntryElement> MarketDataEntryElementPtr;
    
    typedef std::map<char, MarketDataEntryElementPtr> MarketDataEntry;
    typedef std::shared_ptr<MarketDataEntry> MarketDataEntryPtr;
    
    typedef std::map<Instrument, MarketDataEntryPtr> InstrumentMarketData;
    typedef std::shared_ptr<InstrumentMarketData> InstrumentMarketDataPtr;
    
    typedef std::map<FIX::SessionID, InstrumentMarketDataPtr> FixSessionMarketData;
    typedef std::shared_ptr<FixSessionMarketData> FixSessionMarketDataPtr;
    
    class Market
    {
    public:
        
        Market( SimulatorATS::OrderManagerPtr orderManagerPtr ) :
            _fixSessionMarketDataPtr( std::make_shared<FixSessionMarketData>() ),
            _orderManagerPtr ( orderManagerPtr )
        {
            
        }
        
        static std::string getNextOrderId( const Instrument& instrument )
        {
            std::stringstream order_id_stream;

            FIX::UtcTimeStamp utcTime;
            order_id_stream << utcTime.m_time << ":" << ++m_order_index << ":" << instrument.securityExchange << ":" << instrument.symbol;

            return order_id_stream.str();
        }

        
        void insertMarketDataEntry( const FIX::SessionID&, const Instrument&, MarketDataEntryPtr& );
        
        void aggressOnBothSidesAndSubmitRandomOrders();
        
        void printMarketData();
        
        void submitOrder(const FIX::SessionID& sessionID,
                                 const Instrument& instrument,
                                 const FIX::Side& side,
                                 const FIX::OrderQty& orderQty,
                                 const FIX::Price& price
                                 );
        
        bool updateOrder( const FIX::SessionID& sessionID,
                         const Instrument& instrument,
                         const FIX::ClOrdID& clOrdId,
                         const FIX::CumQty& cumQty,
                         const FIX::AvgPx& avgPx,
                         const FIX::OrdStatus& orderStatus
                         );
        
        void cancelOrder( const FIX::SessionID& sessionID,
                          const Instrument& instrument,
                          const std::string& clOrderId,
                         const char side );
        
        void massCancelRequest(const FIX::SessionID& sessionID);
    
        
        FixSessionMarketDataPtr _fixSessionMarketDataPtr;
        FIX::Mutex _mutex;
        
        static uint32_t m_order_index;

        
        SimulatorATS::OrderManagerPtr _orderManagerPtr;
    };
    
    typedef std::shared_ptr<Market> MarketPtr;

};


#endif /* Market_hpp */
