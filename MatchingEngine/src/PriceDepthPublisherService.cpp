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

#include "PriceDepthPublisherService.hpp"
#include "MarketDataIncrementalRefreshTypeSupportImpl.h"
#include <LoggerHelper.h>
#include <MarketDataIncrementalRefreshLogger.hpp>
#include <map>

using namespace DistributedATS;

PriceDepthPublisherService::PriceDepthPublisherService(
                                                       DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshDataWriter_var
        market_data_incremental_refresh_dw,
        PriceDepthPublisherQueuePtr price_depth_publisher_queue_ptr,
        int price_depth_pub_interval)
    : _price_depth_publisher_queue_ptr( price_depth_publisher_queue_ptr ),
        _market_data_incremental_refresh_dw(market_data_incremental_refresh_dw),
        _price_depth_pub_interval(price_depth_pub_interval) {}

int PriceDepthPublisherService::svc(void) {
  std::map<std::string,
           std::shared_ptr<
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>>
      latestMarketDataUpdates;

  ACE_Time_Value udelay(0, _price_depth_pub_interval);
  struct timespec ts = udelay;

  while (1) {
    if (_price_depth_publisher_queue_ptr->message_count() == 0) {
      ACE_OS::nanosleep(&ts);
      continue;
    };

    ACE_Message_Block *incrementalRefreshMessage = 0;

    //
    // Iterate through the queue and get the latest update
    //

    std::cout << "Queue Size : "
              << _price_depth_publisher_queue_ptr->message_count()
              << std::endl;

    while (_price_depth_publisher_queue_ptr->message_count()) {
        _price_depth_publisher_queue_ptr->dequeue_head(
          incrementalRefreshMessage);

      market_data_update *mdUpdate =
          (market_data_update *)incrementalRefreshMessage->rd_ptr();

      std::shared_ptr<
        DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>
          mdIncrementalDataRefresh(mdUpdate->priceDepth);

      latestMarketDataUpdates[mdUpdate->symbol] = mdIncrementalDataRefresh;

      incrementalRefreshMessage->release();
    }

      DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh
        chunkedIncrementalMarketDataRefresh;

    chunkedIncrementalMarketDataRefresh.m_Header.SenderCompID =
        CORBA::string_dup("MATCHING_ENGINE");
    chunkedIncrementalMarketDataRefresh.m_Header.MsgType =
        CORBA::string_dup("X");

    int max_chunk_size = 10;

    CORBA::ULong chunk_size =
        latestMarketDataUpdates.size() > max_chunk_size
            ? max_chunk_size
            : (CORBA::ULong)latestMarketDataUpdates.size();

    chunkedIncrementalMarketDataRefresh.c_NoMDEntries.length(chunk_size * 14);

    int market_data_update_index = 0;
    int chunk_index = 0;

    for (auto marketDataUpdate : latestMarketDataUpdates) {
      for (int md_index = 0; md_index < 14; md_index++) {
        chunkedIncrementalMarketDataRefresh.c_NoMDEntries[chunk_index++] =
            (*marketDataUpdate.second).c_NoMDEntries[md_index];
      }

      if ((++market_data_update_index) % max_chunk_size == 0 ||
          market_data_update_index == latestMarketDataUpdates.size()) {
        LoggerHelper::log_debug<
            std::stringstream, MarketDataIncrementalRefreshLogger,
          DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>(
            chunkedIncrementalMarketDataRefresh,
            "MarketDataIncrementalRefresh");

        std::cout << "Publishing chunk of "
                  << chunkedIncrementalMarketDataRefresh.c_NoMDEntries.length()
                  << "updates" << std::endl;

        std::stringstream ss;
        MarketDataIncrementalRefreshLogger::log(
            ss, chunkedIncrementalMarketDataRefresh);

        int ret = _market_data_incremental_refresh_dw->write(
            chunkedIncrementalMarketDataRefresh, NULL);

        if (ret != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t|%D) ERROR: MarketDataIncrementalRefresh "
                              "write returned %d.\n"),
                     ret));
        }

        chunk_index = 0;

        CORBA::ULong next_chunk_size =
            latestMarketDataUpdates.size() - market_data_update_index >
                    max_chunk_size
                ? max_chunk_size
                : (CORBA::ULong)(latestMarketDataUpdates.size() -
                                 market_data_update_index);

        chunkedIncrementalMarketDataRefresh.c_NoMDEntries.length(
            next_chunk_size * 14);
      }
    }

    latestMarketDataUpdates.clear();

    ACE_OS::nanosleep(&ts);
  }

  return 0;
};
