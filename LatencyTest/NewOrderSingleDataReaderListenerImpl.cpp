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

#include "NewOrderSingleDataReaderListenerImpl.h"
#include <iostream>
#include <vector>

#include <quickfix/FixValues.h>

using namespace LatencyTest;

NewOrderSingleDataReaderListenerImpl::~NewOrderSingleDataReaderListenerImpl() {
	// TODO Auto-generated destructor stub

} /* namespace MatchingEngine */


void NewOrderSingleDataReaderListenerImpl::on_data_available( eprosima::fastdds::dds::DataReader* reader)
{
    
    DistributedATS_NewOrderSingle::NewOrderSingle new_order_single;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&new_order_single, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            
            std::string clientOrderId = new_order_single.ClOrdID();
            
            m_pLatencyStatsPtr->insertStats(clientOrderId,
                                            OrderHopLatency::NEW_ORDER_SINGLE_DDS);
            
        }
    }
}

