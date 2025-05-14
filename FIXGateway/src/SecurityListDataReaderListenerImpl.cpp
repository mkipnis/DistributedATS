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
#include "Application.hpp"
#include <SecurityListAdapter.hpp>
#include <SecurityListLogger.hpp>
#include <quickfix/fix44/SecurityList.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>


namespace DistributedATS {

auto const security_list_processor = [] (DistributedATS::DATSApplication &application, DistributedATS_SecurityList::SecurityList& securityList)
{
    /*
    std::stringstream ss;
    SecurityListLogger::log(ss, securityList);
    LOG4CXX_INFO(logger, "SecurityList : [" <<  ss.str() << "]");
     */

    FIX44::SecurityList securityListMessage;

    securityList.fix_header().SendingTime(0); // this is precision;

    securityList.fix_header().TargetCompID(securityList.DATS_Destination());
    securityList.fix_header().SenderCompID(securityList.DATS_DestinationUser());
    
    HeaderAdapter::DDS2FIX(securityList.fix_header(), securityListMessage.getHeader());

    FIX::SecurityReqID securityReqID(securityList.SecurityReqID());
    securityListMessage.setField(securityReqID);

    FIX::SecurityResponseID securityResponseID(
        securityList.SecurityResponseID());
    securityListMessage.setField(securityResponseID);

    FIX::SecurityRequestResult securityRequestResult(
        securityList.SecurityRequestResult());
    securityListMessage.setField(securityRequestResult);

    for (int sec_index = 0; sec_index < securityList.c_NoRelatedSym().size(); sec_index++)
    {

        FIX44::SecurityList::NoRelatedSym relatedSymbol;
        
        std::string instrument =
          securityList.c_NoRelatedSym()[sec_index].Symbol();
        std::string exchange =
          securityList.c_NoRelatedSym()[sec_index].SecurityExchange();

        std::string ref_data = securityList.c_NoRelatedSym()[sec_index].Text();

        FIX::Symbol symbol(instrument);
        relatedSymbol.setField(symbol);

        FIX::SecurityExchange securityExchange(exchange);
        relatedSymbol.setField(securityExchange);

        if ( ref_data.size() > 0 )
        {
            FIX::Text text(ref_data);
            relatedSymbol.setField(text);
        }

        securityListMessage.addGroup(relatedSymbol);
    }
    

    DistributedATS::DATSApplication::publishToClient(securityListMessage);

};

SecurityListDataReaderListenerImpl::SecurityListDataReaderListenerImpl(DistributedATS::DATSApplication &application)
        : _processor(application, security_list_processor, "SecurityListDataReaderListenerImpl" )
{
};


void SecurityListDataReaderListenerImpl::on_data_available(
                    eprosima::fastdds::dds::DataReader* reader) {
    
    DistributedATS_SecurityList::SecurityList security_list;
    eprosima::fastdds::dds::SampleInfo info;
    if (reader->take_next_sample(&security_list, &info) == eprosima::fastdds::dds::RETCODE_OK)
    {
        if (info.valid_data)
        {
            std::stringstream ss;
            SecurityListLogger::log(ss, security_list);
            LOG4CXX_INFO(logger, "SecurityList : [" <<  ss.str() << "]");
                
            _processor.enqueue_dds_message(security_list);
        }
    }
  
}

} /* namespace DistributedATS */
