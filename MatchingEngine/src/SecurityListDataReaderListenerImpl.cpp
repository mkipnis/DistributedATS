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

#include "SecurityListDataReaderListenerImpl.h"
#include <SecurityList.h>
#include <SecurityListLogger.hpp>


#include <LoggerHelper.h>


namespace DistributedATS {

SecurityListDataReaderListenerImpl::SecurityListDataReaderListenerImpl(
    market_ptr marketPtr)
    : _marketPtr(marketPtr) {
  // TODO Auto-generated constructor stub
}

SecurityListDataReaderListenerImpl::~SecurityListDataReaderListenerImpl() {
  // TODO Auto-generated destructor stub
}

void SecurityListDataReaderListenerImpl::on_data_available(
     eprosima::fastdds::dds::DataReader* reader)
{
    DistributedATS_SecurityList::SecurityList security_list;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (_marketPtr->is_ready_to_trade())
        return;
    
    if (reader->take_next_sample(&security_list, &info) == ReturnCode_t::RETCODE_OK)
    {
        
        if ( security_list.header().TargetCompID().compare(_marketPtr->getMarketName()) == 0 )
        {
            std::stringstream ss;
            SecurityListLogger::log(ss, security_list);
            LOG4CXX_INFO(logger, "SecurityList : [" <<  ss.str() << "]");
            
            for (uint32_t sec_index = 0;
                 sec_index < security_list.c_NoRelatedSym().size(); sec_index++) {
                std::string instrument =
                security_list.c_NoRelatedSym()[sec_index].Symbol();
                
                _marketPtr->addBook(instrument, true);
            }
            
            // request to recieve opening price
            _marketPtr->publishMarketDataRequest();
        }
    }
}
} /* namespace DistributedATS */
