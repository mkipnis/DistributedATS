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

#include <boost/lockfree/spsc_queue.hpp>
#include "Market.h"

#include "PriceDepthPublisherService.hpp"
#include <LoggerHelper.h>
#include <MarketDataIncrementalRefreshLogger.hpp>
#include <map>
#include <thread>
#include <chrono>


using namespace DistributedATS;


PriceDepthPublisherService::PriceDepthPublisherService(
                                                       eprosima::fastdds::dds::DataWriter*
        market_data_incremental_refresh_dw,
        PriceDepthPublisherQueuePtr price_depth_publisher_queue_ptr,
        int price_depth_pub_interval)
    : _price_depth_publisher_queue_ptr( price_depth_publisher_queue_ptr ),
        _market_data_incremental_refresh_dw(market_data_incremental_refresh_dw),
        _price_depth_pub_interval(price_depth_pub_interval)
{
    
    std::atomic_init(&_is_running, true);
    
    _publisher_thread = std::thread(&PriceDepthPublisherService::service, this);
    
}

PriceDepthPublisherService::~PriceDepthPublisherService()
{
    std::atomic_init(&_is_running, false);
    _publisher_thread.join();
};

int PriceDepthPublisherService::service()
{
  while (_is_running) {
    if (_price_depth_publisher_queue_ptr->empty()) {
        
      std::this_thread::sleep_for(std::chrono::duration<long double, std::micro>(_price_depth_pub_interval));
      continue;
    };

    //
    // Iterate through the queue and get the latest update
    //

    /*std::cout << "Queue Size : "
              << _price_depth_publisher_queue_ptr->size()
              << std::endl;*/
      
      std::shared_ptr<DistributedATS::MarketDataUpdate> market_data_update;
      
      std::map<std::string,
        DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>
          latestMarketDataUpdates;
       
      
          while (_price_depth_publisher_queue_ptr->pop(market_data_update))
          {
              latestMarketDataUpdates[market_data_update->symbol] = market_data_update->priceDepth;
              
              std::stringstream ss;
              MarketDataIncrementalRefreshLogger::log(ss, latestMarketDataUpdates[market_data_update->symbol]);
              LOG4CXX_INFO(logger, "MarketDataIncrementalRefresh : [" <<  ss.str() << "]");
              std::cout << "Update : " << ss.str() << std::endl;
              
          }
      

    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
        chunkedIncrementalMarketDataRefresh;

    chunkedIncrementalMarketDataRefresh.DATS_Source("MATCHING_ENGINE");
    chunkedIncrementalMarketDataRefresh.fix_header().MsgType("X");

    int max_chunk_size = 10;

    auto chunk_size =
        latestMarketDataUpdates.size() > max_chunk_size
            ? max_chunk_size
            : latestMarketDataUpdates.size();

    chunkedIncrementalMarketDataRefresh.c_NoMDEntries().resize(chunk_size * 14);

    int market_data_update_index = 0;
    int chunk_index = 0;

    for (auto marketDataUpdate : latestMarketDataUpdates)
    {
      for (int md_index = 0; md_index < 14; md_index++)
      {
        chunkedIncrementalMarketDataRefresh.c_NoMDEntries()[chunk_index++] =
            (marketDataUpdate.second).c_NoMDEntries()[md_index];
      }

      if ((++market_data_update_index) % max_chunk_size == 0 ||
          market_data_update_index == latestMarketDataUpdates.size()) {
        LoggerHelper::log_debug<
            std::stringstream, MarketDataIncrementalRefreshLogger,
          DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>(
            logger,
            chunkedIncrementalMarketDataRefresh,
            "MarketDataIncrementalRefresh");

        std::cout << "Publishing chunk of " << chunkedIncrementalMarketDataRefresh.c_NoMDEntries().size() << " updates" << std::endl;

        std::stringstream ss;
        MarketDataIncrementalRefreshLogger::log(
            ss, chunkedIncrementalMarketDataRefresh);

        int ret = _market_data_incremental_refresh_dw->write(
            &chunkedIncrementalMarketDataRefresh);
          
          if (ret != eprosima::fastdds::dds::RETCODE_OK) {
              LOG4CXX_ERROR(logger, "MarketDataIncrementalRefresh :" << ret);
          }

      }
    }

    latestMarketDataUpdates.clear();

    std::this_thread::sleep_for(std::chrono::duration<long double, std::micro>(_price_depth_pub_interval));
  }

  return 0;
};
