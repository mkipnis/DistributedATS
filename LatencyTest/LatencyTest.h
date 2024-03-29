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

#pragma once

#include <iostream>
#include <time.h>
#include <map>
#include <quickfix/Mutex.h>

namespace LatencyTest
{

    // All hops will include application and network latencies
    enum OrderHopLatency : int
    {
        NEW_ORDER_SINGLE_FIX = 0, // timestamp of order going out from LatencyTest Client
        NEW_ORDER_SINGLE_DDS, // New order single converted from FIX to DDS and sent to matching engine
        EXECUTION_REPORT_DDS, // Execution Report produced by matching engine and published to FIX Gateway
        EXECUTION_REPORT_FIX // Execution Report converted from DDS to FIX and consumed by LatencyTest Client
    };

    enum OrderHopLatencyStats : int
    {
        FIX_NEW_ORDER_SINGLE = 0,
        MATCHING_ENGINE,
        DDS_EXECUTION_REPORT,
        ROUND_TRIP,
    };


    //typedef std::map<OrderHopLatency, timeval> OrderHopLatencyMap;
    //typedef std::shared_ptr<OrderHopLatencyMap> OrderHopLatencyMapPtr;

    struct order_latency_stats
    {
        order_latency_stats()
        {
    
            std::get<NEW_ORDER_SINGLE_FIX>(_order_hop_timestamps) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
            
            std::get<NEW_ORDER_SINGLE_DDS>(_order_hop_timestamps) =
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
            
            std::get<EXECUTION_REPORT_DDS>(_order_hop_timestamps) =
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
            
            std::get<EXECUTION_REPORT_FIX>(_order_hop_timestamps) =
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
        };
    
        void insertStat( OrderHopLatency orderHopLatency )
        {
            switch(orderHopLatency) {
                case NEW_ORDER_SINGLE_FIX: std::get<NEW_ORDER_SINGLE_FIX>(_order_hop_timestamps) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
                case NEW_ORDER_SINGLE_DDS: std::get<NEW_ORDER_SINGLE_DDS>(_order_hop_timestamps) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
                case EXECUTION_REPORT_DDS: std::get<EXECUTION_REPORT_DDS>(_order_hop_timestamps) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
                case EXECUTION_REPORT_FIX: std::get<EXECUTION_REPORT_FIX>(_order_hop_timestamps) = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock().now().time_since_epoch());
            }
        };
        
        void getLatencyStats( std::tuple<long,long,long,long>& latency_tuple )
        {
            FIX::Locker l( _mutex );

            auto new_order_single_timestamp_fix = std::get<NEW_ORDER_SINGLE_FIX>(_order_hop_timestamps);
            auto new_order_single_timestamp_dds = std::get<NEW_ORDER_SINGLE_DDS>(_order_hop_timestamps);
    

            std::get<FIX_NEW_ORDER_SINGLE>(latency_tuple) = std::chrono::duration_cast<std::chrono::microseconds>(new_order_single_timestamp_dds - new_order_single_timestamp_fix).count();

            auto execution_report_dds = std::get<EXECUTION_REPORT_DDS>(_order_hop_timestamps);
            
            std::get<MATCHING_ENGINE>(latency_tuple) = std::chrono::duration_cast<std::chrono::microseconds>(execution_report_dds - new_order_single_timestamp_dds).count();
            
            auto execution_report_fix =
                std::get<EXECUTION_REPORT_FIX>(_order_hop_timestamps);

            std::get<DDS_EXECUTION_REPORT>(latency_tuple) = std::chrono::duration_cast<std::chrono::microseconds>(execution_report_fix - execution_report_dds).count();
            
            std::get<ROUND_TRIP>(latency_tuple)  = std::chrono::duration_cast<std::chrono::microseconds>(execution_report_fix - new_order_single_timestamp_fix).count();

        }
    
        std::tuple<std::chrono::microseconds,
                std::chrono::microseconds,
                std::chrono::microseconds,
                std::chrono::microseconds > _order_hop_timestamps;
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
                             std::tuple<long,long,long,long>& latency_tuple
                             )
        {
            FIX::Locker l( _mutex );

            auto latencyStats = _orderLatencyStatsMapPtr->find( orderId );
            
            if ( latencyStats != _orderLatencyStatsMapPtr->end() )
            {
                latencyStats->second->getLatencyStats( latency_tuple );
            }
        }

        FIX::Mutex _mutex;
        OrderLatencyStatsMapPtr _orderLatencyStatsMapPtr;
    };

typedef std::shared_ptr<latency_stats> LatencyStatsPtr;
};


