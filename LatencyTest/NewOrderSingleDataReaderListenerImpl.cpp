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

#include "NewOrderSingleDataReaderListenerImpl.h"
#include <iostream>
#include <vector>

#include <quickfix/FixValues.h>

using namespace LatencyTest;

NewOrderSingleDataReaderListenerImpl::~NewOrderSingleDataReaderListenerImpl() {
	// TODO Auto-generated destructor stub

} /* namespace MatchingEngine */


void NewOrderSingleDataReaderListenerImpl::on_data_available( DDS::DataReader_ptr reader) throw (CORBA::SystemException)
{
    try
    {
        DistributedATS_NewOrderSingle::NewOrderSingleDataReader_var new_order_single_dr = DistributedATS_NewOrderSingle::NewOrderSingleDataReader::_narrow(reader);

        if (CORBA::is_nil ( new_order_single_dr.in() ) )
        {
            std::cerr << "NewOrderSingleDataReaderListenerImpl::on_data_available: _narrow failed." << std::endl;
            ACE_OS::exit(1);
        }

        while( true )
        {
            DistributedATS_NewOrderSingle::NewOrderSingle new_order_single;
            DDS::SampleInfo si ;
            DDS::ReturnCode_t status = new_order_single_dr->take_next_sample( new_order_single, si );

            if (status == DDS::RETCODE_OK)
            {
                if ( !si.valid_data )
                    continue;

               	std::string clientOrderId = new_order_single.ClOrdID.in();
                
                m_pLatencyStatsPtr->insertStats(clientOrderId,
                                                OrderHopLatency::NEW_ORDER_SINGLE_DDS);

            } else if (status == DDS::RETCODE_NO_DATA) {
                break;
            } else {
                std::cerr << "ERROR: read DATS::Logon: Error: " <<  status << std::endl;
            }
        }

    } catch (CORBA::Exception& e) {
        std::cerr << "Exception caught in read:" << std::endl << e << std::endl;
        ACE_OS::exit(1);
    }


}
