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

#include "Market.hpp"
#include "Application.h"

#include <quickfix/FixValues.h>

using namespace SimulatorATS;

uint32_t Market::m_order_index = 0;

void Market::insertMarketDataEntry( const FIX::SessionID& sessionId, const Instrument& instrument,
                                   MarketDataEntryPtr& marketDataSnapshot )
{
    FIX::Locker lock(_mutex);
    
    auto fixSessionMarketData = _fixSessionMarketDataPtr->find( sessionId );
    
    if ( fixSessionMarketData == _fixSessionMarketDataPtr->end())
        fixSessionMarketData = ( _fixSessionMarketDataPtr->emplace( sessionId, std::make_shared< InstrumentMarketData > () ) ).first;
    
    (*fixSessionMarketData->second)[instrument] = marketDataSnapshot;
    
};



void Market::printMarketData()
{
    FIX::Locker lock(_mutex);

    std::cout << "----- Market Data -----" << std::endl;

    for ( auto& sessionInstrumentMarketData : *_fixSessionMarketDataPtr )
    {
        std::cout << "Session : " << sessionInstrumentMarketData.first << std::endl;
        
        for ( auto& instrumentMarketData : *sessionInstrumentMarketData.second )
        {
            std::cout << "\tInstrument : " << instrumentMarketData.first << std::endl;
            
            for ( auto& marketData : *instrumentMarketData.second )
            {
                std::cout << "\t\tMarket Data Entry Type : " << marketData.first << std::endl;

                for ( auto& marketDataEntity : *marketData.second )
                {
                    for ( auto& marketDataEntityEntry : *marketDataEntity )
                    {
                        std::cout << "\t\t\t" << marketDataEntityEntry.first << ":" << marketDataEntityEntry.second << std::endl;
                    }
                }
            }
        }
    }
    
    std::cout << "---------------" << std::endl;

};


void Market::aggressOnBothSidesAndSubmitRandomOrders()
{
    FIX::Locker lock(_mutex);
    
    for ( auto& sessionInstrumentMarketData : *_fixSessionMarketDataPtr )
    {
        std::cout << sessionInstrumentMarketData.first << std::endl;
        std::cout << "---------------" << std::endl;
        
        int worst_bid, worst_ask, open_price;
        int total_bid_quantity, total_ask_quantity;
        
        worst_bid = worst_ask = open_price = total_bid_quantity = total_ask_quantity = 0;
        
        for ( auto& instrumentMarketData : *sessionInstrumentMarketData.second )
        {
            std::cout << "Instrument : " << instrumentMarketData.first.securityExchange << " : " <<  instrumentMarketData.first.symbol << std::endl;
            
            auto& instrument = instrumentMarketData.first;
            
            for ( auto& marketData : *instrumentMarketData.second )
            {
                std::cout << "\tMarket Data Entry Type : " << marketData.first << std::endl;
                
                auto& mdEntryType = marketData.first;
                
                for ( auto& marketDataEntity : *marketData.second )
                {
                    for ( auto& marketDataEntityEntry : *marketDataEntity )
                    {
                        std::cout << "\t\t" << marketDataEntityEntry.first << ":" << marketDataEntityEntry.second << std::endl;
                        
                        auto& mdEntryPx = marketDataEntityEntry.first;
                        auto& mdEntrySize = marketDataEntityEntry.second;
                        
                        if ( mdEntryType == FIX::MDEntryType_BID )
                        {
                            if ( worst_bid == 0 || ( mdEntryPx> 0 && worst_bid > mdEntryPx ) )
                                worst_bid = mdEntryPx;
                         
                            total_bid_quantity += mdEntrySize;
                         
                        } else if ( mdEntryType == FIX::MDEntryType_OFFER )
                        {
                            if ( worst_ask == 0 || ( mdEntryPx>0 && worst_ask < mdEntryPx ))
                                worst_ask = mdEntryPx;
                         
                            total_ask_quantity += mdEntrySize;
                         
                        } else if ( mdEntryType == FIX::MDEntryType_OPENING_PRICE )
                        {
                            open_price = mdEntryPx;
                        }
                    }
                }
                
                // aggress on bids
                if ( total_bid_quantity > 0 )
                    submitOrder( sessionInstrumentMarketData.first, instrument,
                                FIX::Side_SELL, rand() % total_bid_quantity, worst_bid );
                
                // aggress on asks
                if ( total_ask_quantity > 0 )
                    submitOrder( sessionInstrumentMarketData.first, instrument,
                                FIX::Side_BUY, rand() % total_ask_quantity, worst_ask);
                
                double random_price_move_factor = 1 + ( ( rand()%10 ) / 100.0 ) ;

                // submit passive bid
                submitOrder( sessionInstrumentMarketData.first, instrument, FIX::Side_BUY,
                             (rand()%100), open_price / random_price_move_factor );

                // submit passive ask
                submitOrder( sessionInstrumentMarketData.first, instrument, FIX::Side_SELL,
                            (rand()%100), open_price * random_price_move_factor ) ;
                


            }
        }
        
        std::cout << "---------------" << std::endl;
    }
    
};


void Market::submitOrder(const FIX::SessionID& sessionID,
                              const Instrument& instrument,
                              const FIX::Side& side,
                              const FIX::OrderQty& orderQty,
                              const FIX::Price& price
                              )
{
    
    const FIX::SecurityExchange securityExchange( instrument.securityExchange );
    const FIX::Symbol symbol( instrument.symbol );
    
    FIX::ClOrdID clOrdID( Market::getNextOrderId( instrument ) );
                                    
    std::cout << "Lets submit an order :"  << securityExchange << ":"
        << symbol << ":" << side << ":" << price  << ":" <<  orderQty << std::endl;
    
    FIX::OrdType ordType;
    
    FIX44::NewOrderSingle newOrderSingle(clOrdID, side, FIX::TransactTime(), FIX::OrdType_LIMIT);
    
    newOrderSingle.set(FIX::HandlInst('1'));
    newOrderSingle.set(symbol);
    newOrderSingle.set(securityExchange);
    newOrderSingle.set(orderQty);
    newOrderSingle.set(FIX::TimeInForce(FIX::TimeInForce_GOOD_TILL_CANCEL));
    newOrderSingle.set(price);
    
    FIX::Header& header = newOrderSingle.getHeader();
    
    FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
    FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
    header.setField(senderCompID);
    header.setField(targetCompID);
    
    FIX::Session::sendToTarget(newOrderSingle);

    _orderManagerPtr->insertNewOrder( sessionID, instrument, clOrdID, side, price, orderQty );
}


void Market::cancelOrder( const FIX::SessionID& sessionID,
                  const Instrument& instrument,
                  const std::string& clOrderId,
                 const char side )
{
    FIX::OrigClOrdID origClOrdID(clOrderId);
    FIX::ClOrdID clOrdID( Market::getNextOrderId( instrument  ));
    FIX::Side fixSide( side );
    FIX::TransactTime transactTime;
    
    FIX44::OrderCancelRequest orderCancelRequest(origClOrdID, clOrdID, fixSide, transactTime);
    
    FIX::Symbol symbol( instrument.symbol);
    orderCancelRequest.setField( symbol );
    FIX::SecurityExchange securityExchange( instrument.securityExchange );
    orderCancelRequest.setField(securityExchange);
    
    FIX::Header& header = orderCancelRequest.getHeader();
      
    FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
    FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
    header.setField(senderCompID);
    header.setField(targetCompID);
    
    FIX::Session::sendToTarget(orderCancelRequest);

}


void Market::massCancelRequest(const FIX::SessionID& sessionID)
{
    auto pSession = FIX::Session::lookupSession(sessionID);
    
    if ( !pSession || !pSession->isLoggedOn() )
        return;
    
    FIX44::OrderMassCancelRequest orderMassCancelRequest(Market::getNextOrderId( Instrument("","") ), FIX::MassCancelRequestType_CANCEL_ALL_ORDERS, FIX::TransactTime());


    FIX::Header& header = orderMassCancelRequest.getHeader();

    FIX::SenderCompID senderCompID(sessionID.getSenderCompID());
    FIX::TargetCompID targetCompID(sessionID.getTargetCompID());
    header.setField(senderCompID);
    header.setField(targetCompID);

    FIX::Session::sendToTarget(orderMassCancelRequest);
}




