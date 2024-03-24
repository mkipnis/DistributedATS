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

#ifndef DATS_APPLICATION_H
#define DATS_APPLICATION_H

#include <stdio.h>
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Queue.h>
#include <quickfix/SessionFactory.h>
#include <memory>
#include <map>

#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/ExecutionReport.h>
#include <quickfix/fix44/OrderCancelRequest.h>
#include <quickfix/fix44/OrderCancelReject.h>
#include <quickfix/fix44/OrderCancelReplaceRequest.h>
#include <quickfix/fix44/MarketDataRequest.h>
#include <quickfix/fix44/MarketDataIncrementalRefresh.h>
#include <quickfix/fix44/OrderMassCancelRequest.h>
#include <quickfix/fix44/SecurityListRequest.h>
#include <quickfix/fix44/SecurityList.h>
#include <quickfix/fix44/SecurityListRequest.h>
#include <quickfix/fix44/MarketDataSnapshotFullRefresh.h>
#include <quickfix/fix44/OrderMassStatusRequest.h>
#include <quickfix/fix44/OrderMassCancelReport.h>

#include "Market.hpp"

#include <atomic>
#include <condition_variable>


namespace SimulatorATS
{

    enum Operation
    {
        AGGRESS_ON_OPEN_ORDERS_AND_SUBMIT_MORE_ORDERS,
        CANCEL_PARTIALLY_FILLED_ORDERS,
        MASS_CANCEL
    };


    class SocketConnection;
    class SocketAcceptor;
    typedef FIX::Queue<FIX::Message> FIXMessageQueue;
    typedef std::map<const FIX::SessionID, FIX::Message> SessionReLoginMap;
    
    class Application
    : public FIX::Application, public FIX::MessageCracker
    {
        private:
        
            void onCreate( const FIX::SessionID& ) {}
            void onLogon( const FIX::SessionID& sessionID );
            void onLogout( const FIX::SessionID& sessionID );
            void toAdmin( FIX::Message&, const FIX::SessionID& );
            void toApp( FIX::Message&, const FIX::SessionID& ) throw( FIX::DoNotSend );
            void fromAdmin( const FIX::Message&, const FIX::SessionID& )
                throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {};

            void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
                throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

            void onMessage(const FIX44::SecurityList&, const FIX::SessionID&);
            void onMessage( const FIX44::ExecutionReport&, const FIX::SessionID& );
            void onMessage( const FIX44::OrderCancelReject&, const FIX::SessionID& );
            void onMessage( const FIX44::MarketDataIncrementalRefresh&, const FIX::SessionID&);
            void onMessage( const FIX44::MarketDataSnapshotFullRefresh&, const FIX::SessionID&);
            void onMessage( const FIX44::OrderMassCancelReport&, const  FIX::SessionID&);

        public:
        
            Application( const FIX::SessionSettings& sessionSettings, SimulatorATS::MarketPtr& marketPtr,
                        SimulatorATS::OrderManagerPtr& orderManagerPtr )
                : m_sessionSettings( sessionSettings ), m_marketPtr( marketPtr ), m_orderManagerPtr( orderManagerPtr )
                {
                };

            void SecurityListRequest( const FIX::SessionID& sessionID, const FIX::SecurityReqID& secListRequestID );
            void OrderMassStatusRequest( const FIX::SessionID& sessionID, const FIX::MassStatusReqID& massStatusReqID );
        
            static const FIX::SessionID SessionIDFromMessage( const FIX::Message&, const std::string& sessionQualifier = "" );
        
            bool publicCreateSession(const FIX::SessionID& );
            bool enqueueOutgoingMessage( FIX::Message& );
            bool insertReLogin( const FIX::SessionID&, FIX::Message& );

            void onDisconnect( const FIX::SessionID& sessionID );

        public:
            bool enqueueLogin(const FIX::Message&);
            bool getNextLogin(FIX::Message&);

        public:
            bool processNextMessage(FIX::Message& nextMessage);

            void run();
        
        private:
            FIXMessageQueue m_outgoingMessageQueue;
            FIXMessageQueue m_pendingLoginQueue;
            SessionReLoginMap m_sessionReloginMap;
        
            FIX::SessionSettings m_sessionSettings;
            SimulatorATS::MarketPtr m_marketPtr;
            SimulatorATS::OrderManagerPtr m_orderManagerPtr;
        
            std::condition_variable _login_cv;
            std::mutex _login_mutex;
    };

};
#endif /* DATS_APPLICATION_H */
