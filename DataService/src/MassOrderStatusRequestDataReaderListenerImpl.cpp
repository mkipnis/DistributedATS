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

#include "MassOrderStatusRequestDataReaderListenerImpl.h"

#include <iostream>

#include <OrderMassStatusRequestLogger.hpp>

namespace DistributedATS {

MassOrderStatusRequestDataReaderListenerImpl::MassOrderStatusRequestDataReaderListenerImpl( 
	ACE_Message_Queue<ACE_MT_SYNCH>*  orderMassStatusRequestQueue
)
: _orderMassStatusRequestQueue ( orderMassStatusRequestQueue )
{
	// TODO Auto-generated constructor stub

}

MassOrderStatusRequestDataReaderListenerImpl::~MassOrderStatusRequestDataReaderListenerImpl() {
	// TODO Auto-generated destructor stub
}

void MassOrderStatusRequestDataReaderListenerImpl::on_data_available( DDS::DataReader_ptr reader) throw (CORBA::SystemException)
{
	 try
	 {
		 DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestDataReader_var
				order_mass_status_request_dr = DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestDataReader::_narrow(reader);

		 if (CORBA::is_nil ( order_mass_status_request_dr.in() ) )
		 {
             ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) MassOrderStatusRequestDataReaderListenerImpl::on_data_available: _narrow failed.\n")));
                       
			 ACE_OS::exit(1);
		 }

         while( true )
         {
             DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest orderMassStatusRequest;
             DDS::SampleInfo si ;
             DDS::ReturnCode_t status = order_mass_status_request_dr->take_next_sample( orderMassStatusRequest, si );

             if (status == DDS::RETCODE_OK)
             {
                 if ( !si.valid_data )
                     continue;
                 
                 std::stringstream ss;
                 OrderMassStatusRequestLogger::log(ss, orderMassStatusRequest);
                 ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) OrderMassStatusRequest : %s\n"), ss.str().c_str()));

                 DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest* orderMassStatusRequestPtr =
                                new DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest( orderMassStatusRequest );

                 ACE_Message_Block* msg = new ACE_Message_Block((char*)orderMassStatusRequestPtr, sizeof( DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest ));
                      _orderMassStatusRequestQueue->enqueue_tail(msg);


             } else if (status == DDS::RETCODE_NO_DATA) {
                 break;
             } else {
                 ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) Read OrderMassStatusRequest: %d\n"), status));
             }
         }

	    } catch (CORBA::Exception& e) {

            std::stringstream ss;
            ss << "Exception caught in read:" << std::endl << e << std::endl;
            ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t|%D) DistributedATS_OrderMassStatusRequest %s.\n"), ss.str().c_str()));

            ACE_OS::exit(1);
	    }
}

} /* namespace DistributedATS */
