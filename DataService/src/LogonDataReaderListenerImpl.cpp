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

#include <iostream>
#include "LogonDataReaderListenerImpl.hpp"
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <quickfix/FixValues.h>

#include <LogonLogger.hpp>

#include <sstream>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

static auto logger = log4cxx::Logger::getRootLogger();

using namespace DistributedATS;

void LogonDataReaderListenerImpl::on_data_available( eprosima::fastdds::dds::DataReader* reader )
{

    DistributedATS_Logon::Logon logon;
    eprosima::fastdds::dds::SampleInfo info;
    
    if (reader->take_next_sample(&logon, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            std::stringstream ss;
            LogonLogger::log(ss, logon);
            LOG4CXX_INFO(logger, "Data Reader Logon : [" <<  ss.str() << "]" << logon.header().TargetCompID() );
            
            if (logon.header().TargetCompID().compare("AUTH") == 0)
                _logonQueuePtr->push(std::make_shared<DistributedATS_Logon::Logon>(logon));
            
        }
    }
    
}
