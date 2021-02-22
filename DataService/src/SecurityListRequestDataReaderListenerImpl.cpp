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

#include <iostream>
#include <sstream>
#include "SecurityListRequestDataReaderListenerImpl.hpp"
#include <SecurityListRequestTypeSupportImpl.h>
#include <SecurityListTypeSupportImpl.h>
#include <SecurityListRequestLogger.hpp>
#include <quickfix/FixValues.h>


void SecurityListRequestDataReaderListenerImpl::on_data_available( DDS::DataReader_ptr reader) throw (CORBA::SystemException)
{
    try
    {
        DistributedATS_SecurityListRequest::SecurityListRequestDataReader_var security_list_dr =
            DistributedATS_SecurityListRequest::SecurityListRequestDataReader::_narrow(reader);
        
        if (CORBA::is_nil ( security_list_dr.in() ) )
        {
            ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) SecurityListDataRequestReaderListenerImpl::on_data_available: _narrow failed.\n")));
            ACE_OS::exit(1);
        }
        
        while( true )
        {
            DistributedATS_SecurityListRequest::SecurityListRequest security_list_request;
            DDS::SampleInfo si ;
            DDS::ReturnCode_t status = security_list_dr->take_next_sample( security_list_request, si );
            
            if (status == DDS::RETCODE_OK)
            {
                if ( !si.valid_data )
                    continue;

                std::stringstream ss;
                SecurityListRequestLogger::log(ss, security_list_request);
                ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) SecurityListRequest : %s\n"), ss.str().c_str()));
                
                DistributedATS_SecurityListRequest::SecurityListRequest* securityListRequest =
                    new DistributedATS_SecurityListRequest::SecurityListRequest( security_list_request );

                ACE_Message_Block* msg = new ACE_Message_Block((char*)securityListRequest, sizeof( DistributedATS_SecurityListRequest::SecurityListRequest ));
                _securityListRequestQueue->enqueue_tail(msg);

            } else if (status == DDS::RETCODE_NO_DATA) {
                    break;
            } else {

                ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) Read SecurityListRequest: %d\n"), status));
            }
        }
        
    } catch (CORBA::Exception& e) {
        

        std::stringstream ss;
        ss << "Exception caught in read:" << std::endl << e << std::endl;
        ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) SecurityListRequest: %s.\n"), ss.str().c_str()));

        ACE_OS::exit(1);
    }


}
