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

#include "LatencyTest.h"

#include <quickfix/Mutex.h>

#include <condition_variable>

#include <limits>

namespace LatencyTest
{
    class SocketConnection;
    class SocketAcceptor;
    typedef FIX::Queue<FIX::Message> FIXMessageQueue;
    typedef std::map<const FIX::SessionID, FIX::Message> SessionReLoginMap;
    
    class Application : public FIX::Application, public FIX::MessageCracker
    {
        private:
        
            void onCreate( const FIX::SessionID& ) {}
            void onLogon( const FIX::SessionID& sessionID );
            void onLogout( const FIX::SessionID& sessionID );
            void toAdmin( FIX::Message&, const FIX::SessionID& );
            void toApp( FIX::Message&, const FIX::SessionID& ) throw( FIX::DoNotSend );
            void fromAdmin( const FIX::Message&, const FIX::SessionID& ) throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {};

            void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID ) throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );
            void onMessage( const FIX44::ExecutionReport&, const FIX::SessionID& );
            void onMessage( const FIX44::MarketDataIncrementalRefresh&, const FIX::SessionID&);

        public:
            void submitOrder(const FIX::SessionID& sessionID,
                         const FIX::Symbol& symbol,
                         const FIX::SecurityExchange& securityExchange,
                         const FIX::ClOrdID& clOrdID,
                         const FIX::Side& side,
                         const FIX::OrderQty& orderQty,
                         const FIX::Price& price);

            void cancelOrder(const FIX::SessionID& sessionID,
                             const FIX::Symbol& symbol,
                             const FIX::OrigClOrdID& origClOrdID,
                             const FIX::ClOrdID& clOrdID,
                             const FIX::Side& side);

            void massCancelRequest(const FIX::SessionID& sessionID,
                                   const FIX::ClOrdID& clOrdID);

            void SecurityListRequest(const FIX::SessionID& sessionID,
                                     const FIX::SecurityReqID& secListRequestID);

            void OrderMassStatusRequest(const FIX::SessionID& sessionID,
                                        const FIX::MassStatusReqID& massStatusReqID);
        
            static const FIX::SessionID SessionIDFromMessage(const FIX::Message&, const std::string& sessionQualifier = "");
        
            bool publicCreateSession(const FIX::SessionID& );
            bool enqueueOutgoingMessage( FIX::Message& );
            bool insertReLogin( const FIX::SessionID&, FIX::Message& );

            void onDisconnect( const FIX::SessionID& sessionID );

            std::string m_password;
            static uint32_t _order_index;
            std::string m_last_order_id;
            uint32_t m_number_of_orders;

            std::string getNextOrderId(const std::string& symbol)
            {
                _order_index++;

                std::stringstream order_id_stream;

                FIX::UtcTimeStamp utcTime;
                order_id_stream << utcTime.m_time << ":" << _order_index << ":" << symbol;

                return order_id_stream.str();
            }

        
        public:
            bool enqueueLogin(const FIX::Message&);
            bool getNextLogin(FIX::Message&);

        public:
            bool processNextMessage(FIX::Message& nextMessage);

            Application(FIX::SessionSettings& sessionSettings, LatencyStatsPtr latencyStatsPtr ) : m_sessionSettings( sessionSettings ), m_pLatencyStatsPtr( latencyStatsPtr )
            {};

            void run();
    
        private:
            FIXMessageQueue m_outgoingMessageQueue;
            FIXMessageQueue m_pendingLoginQueue;
            SessionReLoginMap m_sessionReloginMap;
            FIX::SessionSettings m_sessionSettings;

            LatencyStatsPtr m_pLatencyStatsPtr;
        
            std::condition_variable _login_cv;
            std::mutex _login_mutex;
        
        uint32_t _min_latency {std::numeric_limits<uint32_t>::max()};
        uint32_t _max_latency {std::numeric_limits<uint32_t>::min()};
        uint32_t _total_latency {0};
    };

};
#endif /* DATS_APPLICATION_H */
