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

#ifndef __LATENCY_TEST_H__
#define __LATENCY_TEST_H__

#include <iostream>
#include <time.h>
#include <quickfix/Mutex.h>

namespace LatencyTest
{

    // All hops will include application and network latencies
    enum OrderHopLatency
    {
        NEW_ORDER_SINGLE_FIX, // timestamp of order going out from LatencyTest Client
        NEW_ORDER_SINGLE_DDS, // New order single converted from FIX to DDS and sent to matching engine
        EXECUTION_REPORT_DDS, // Execution Report produced by matching engine and published to FIX Gateway
        EXECUTION_REPORT_FIX // Execution Report converted from DDS to FIX and consumed by LatencyTest Client
    };

    typedef std::map<OrderHopLatency, timeval> OrderHopLatencyMap;
    typedef std::shared_ptr<OrderHopLatencyMap> OrderHopLatencyMapPtr;

    struct order_latency_stats
    {
        order_latency_stats() : _orderHopLatencyMapPtr( std::make_shared<OrderHopLatencyMap>() )
        {
            timeval timestamp;
            memset(&timestamp, 0x0, sizeof(timestamp));
            _orderHopLatencyMapPtr->emplace(OrderHopLatency::NEW_ORDER_SINGLE_FIX, timestamp);
            _orderHopLatencyMapPtr->emplace(OrderHopLatency::NEW_ORDER_SINGLE_DDS, timestamp);
            _orderHopLatencyMapPtr->emplace(OrderHopLatency::EXECUTION_REPORT_DDS, timestamp);
            _orderHopLatencyMapPtr->emplace(OrderHopLatency::EXECUTION_REPORT_FIX, timestamp);
        };
    
        void insertStat( OrderHopLatency orderHopLatency )
        {
            FIX::Locker l( _mutex );
            timeval timestamp;
            gettimeofday( &timestamp, NULL );
            (*_orderHopLatencyMapPtr)[orderHopLatency] = timestamp;
        };
        
        void getLatencyStats( long& gatewayNewOrderSingleLatency,
                              long& matchingEngineLatency,
                              long& gatewayExecutionReportLatency,
                              long& roundTripLatency )
        {
            FIX::Locker l( _mutex );

            auto new_order_single_timestamp_fix =
                _orderHopLatencyMapPtr->find(OrderHopLatency::NEW_ORDER_SINGLE_FIX);
            
            auto new_order_single_timestamp_dds =
                _orderHopLatencyMapPtr->find(OrderHopLatency::NEW_ORDER_SINGLE_DDS);

            gatewayNewOrderSingleLatency = (new_order_single_timestamp_dds->second.tv_sec - (long)new_order_single_timestamp_fix->second.tv_sec) * 1000000 +
                    (new_order_single_timestamp_dds->second.tv_usec - (long)new_order_single_timestamp_fix->second.tv_usec);

            auto execution_report_dds =
                _orderHopLatencyMapPtr->find(OrderHopLatency::EXECUTION_REPORT_DDS);
            
            matchingEngineLatency = (execution_report_dds->second.tv_sec - (long)new_order_single_timestamp_dds->second.tv_sec) * 1000000 +
                    (execution_report_dds->second.tv_usec - (long)new_order_single_timestamp_dds->second.tv_usec);
            
            auto execution_report_fix =
                _orderHopLatencyMapPtr->find(OrderHopLatency::EXECUTION_REPORT_FIX);

            gatewayExecutionReportLatency = (execution_report_fix->second.tv_sec - (long)execution_report_dds->second.tv_sec) * 1000000 +
                    (execution_report_fix->second.tv_usec - (long)execution_report_dds->second.tv_usec);

            
            if ( matchingEngineLatency<0 ) // Fix message was recieved before dds exection report -- needs checking, might be resouce starvation if everything runs on the same box
                matchingEngineLatency = gatewayExecutionReportLatency = 0;
            
            roundTripLatency = (execution_report_fix->second.tv_sec - (long)new_order_single_timestamp_fix->second.tv_sec) * 1000000 +
                    (execution_report_fix->second.tv_usec - (long)new_order_single_timestamp_fix->second.tv_usec);

        }
    
        OrderHopLatencyMapPtr _orderHopLatencyMapPtr;
        FIX::Mutex _mutex;
    };

    typedef std::shared_ptr<order_latency_stats> OrderLatencyStatsPtr;
    typedef std::map<std::string, OrderLatencyStatsPtr> OrderLatencyStatsMap;
    typedef std::shared_ptr<OrderLatencyStatsMap> OrderLatencyStatsMapPtr;

    struct latency_stats
    {
        latency_stats() : _orderLatencyStatsMapPtr( std::make_shared< OrderLatencyStatsMap >() )
        {
            
        };
        
        void insertStats( const std::string& orderId, OrderHopLatency orderHopLatency )
        {
            FIX::Locker l( _mutex );

            if ( orderHopLatency == OrderHopLatency::NEW_ORDER_SINGLE_FIX )
            {
                _orderLatencyStatsMapPtr->emplace( orderId, std::make_shared<order_latency_stats>() ).first->second->insertStat( orderHopLatency );
            } else {
                auto latency_stats = _orderLatencyStatsMapPtr->find( orderId );
                
                if ( latency_stats != _orderLatencyStatsMapPtr->end() )
                    latency_stats->second->insertStat( orderHopLatency );
            }
        };
        
        void getLatencyStats( const std::string& orderId,
                             long& gatewayNewOrderSingleLatency,
                             long& matchingEngineLatency,
                             long& gatewayExecutionReportLatency,
                             long& roundTripLatency)
        {
            FIX::Locker l( _mutex );

            auto latencyStats = _orderLatencyStatsMapPtr->find( orderId );
            
            if ( latencyStats != _orderLatencyStatsMapPtr->end() )
            {
                latencyStats->second->getLatencyStats( gatewayNewOrderSingleLatency,
                                              matchingEngineLatency,
                                              gatewayExecutionReportLatency,
                                              roundTripLatency );
            }
        }

        FIX::Mutex _mutex;
        OrderLatencyStatsMapPtr _orderLatencyStatsMapPtr;
    };

typedef std::shared_ptr<latency_stats> LatencyStatsPtr;
};

#endif

