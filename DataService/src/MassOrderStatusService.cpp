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

#include "MassOrderStatusService.h"
#include <ExecutionReportTypeSupportImpl.h>
#include "ExecutionReportDataReaderListenerImpl.h"
#include "MassOrderStatusRequestDataReaderListenerImpl.h"

#include <Common.h>

namespace DistributedATS {

MassOrderStatusService::MassOrderStatusService( std::shared_ptr<distributed_ats_utils::BasicDomainParticipant>
				basicDomainParticipantPtr,
				ACE_Thread_Manager *thr_mgr ) : ACE_Task<ACE_MT_SYNCH> (thr_mgr)
{
	// TODO Auto-generated constructor stub
    
    m_basicDomainParticipantPtr = basicDomainParticipantPtr;
    
    m_executionReports = std::make_shared<ExecutionReports>( ExecutionReports() );

}

MassOrderStatusService::~MassOrderStatusService() {
	// TODO Auto-generated destructor stub
}

void MassOrderStatusService::createOrderMassStatusRequestListener( const std::string& data_service_filter_expression )
{
    DDS::Topic_var order_mass_status_request_topic =  m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestTypeSupport_var,
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestTypeSupportImpl >
        ( ORDER_MASS_STATUS_REQUEST_TOPIC_NAME );
    
    std::string data_service_filter_expression_exec_report =
    data_service_filter_expression + " and m_Header.SenderCompID <> 'DATA_SERVICE'";
    
    
    DDS::ContentFilteredTopic_ptr cft_order_mass_status_request =
    m_basicDomainParticipantPtr->getDomainParticipant()->create_contentfilteredtopic( "MASS_STATUS_REQUEST_FILTER", order_mass_status_request_topic, data_service_filter_expression.c_str(), DDS::StringSeq()); ;
    
    DDS::DataReaderListener_var orderMassStatusRequestDataListener(new DistributedATS::MassOrderStatusRequestDataReaderListenerImpl( msg_queue()  ) );
    m_basicDomainParticipantPtr->createDataReaderListener( cft_order_mass_status_request, orderMassStatusRequestDataListener );

}

void MassOrderStatusService::createExecutionReportListener( const std::string& data_service_filter_expression )
{
    DDS::Topic_var execution_report_topic =  m_basicDomainParticipantPtr->createTopicAndRegisterType
        < DistributedATS_ExecutionReport::ExecutionReportTypeSupport_var, DistributedATS_ExecutionReport::ExecutionReportTypeSupportImpl >
        ( EXECUTION_REPORT_TOPIC_NAME );
    
    std::string data_service_filter_expression_exec_report =
    data_service_filter_expression + " and m_Header.SenderCompID <> 'DATA_SERVICE'";
    
    // Filter out executions report published by it self, incase of mass order status report
    DDS::ContentFilteredTopic_ptr cft_execution_report =
    m_basicDomainParticipantPtr->getDomainParticipant()->create_contentfilteredtopic( "EXECUTION_REPORT_FILTER", execution_report_topic,
                                                                        data_service_filter_expression_exec_report.c_str(),
                                                                        DDS::StringSeq());
    
    
    m_execution_report_dw = m_basicDomainParticipantPtr->createDataWriter< DistributedATS_ExecutionReport::ExecutionReportDataWriter_var, DistributedATS_ExecutionReport::ExecutionReportDataWriter > ( execution_report_topic );
    
    DDS::DataReaderListener_var executionReportDataListener(new DistributedATS::ExecutionReportDataReaderListenerImpl(m_executionReports));
    
    m_basicDomainParticipantPtr->createDataReaderListener( cft_execution_report, executionReportDataListener );
}

int MassOrderStatusService::svc (void)
{
	while(1)
	{
        ACE_Message_Block* messageBlock = 0;
        
        ACE_Time_Value interval (0, 1000);
        
        if ( this->getq (messageBlock, &interval) == 0 )
        {
            DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest* orderMassStatusRequestPtr = (DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest*)messageBlock->rd_ptr();

			 processMassOrderStatusServiceRequest(orderMassStatusRequestPtr);

			 messageBlock->release();
        } else {
            ACE_OS::sleep(interval);
        }
	}
}


OrderToExecutionReportMapPtr MassOrderStatusService::getOrderToExecutionReportMap( std::string& senderCompId )
{
	auto orderIdToExecutionReportMap = m_executionReports->find(senderCompId);

	if ( orderIdToExecutionReportMap != m_executionReports->end() )
		return orderIdToExecutionReportMap->second;

	return NULL;
}
    
bool MassOrderStatusService::processMassOrderStatusServiceRequest( DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest* orderMassStatusRequestPtr )
{

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Received Mass Order Status Request  : %s %s\n"),
               orderMassStatusRequestPtr->m_Header.SenderCompID.in(),
               orderMassStatusRequestPtr->m_Header.TargetCompID.in()));
    
    std::string username = orderMassStatusRequestPtr->m_Header.SenderSubID.in();
    
    OrderToExecutionReportMapPtr orderToExecutionReportMapPtr =
    getOrderToExecutionReportMap( username );
    
    if ( orderToExecutionReportMapPtr != NULL )
    {
        for ( auto& orderToExecReportListPtr : *orderToExecutionReportMapPtr )
        {
            std::cout << "Need to publish execution report for the following order id : " << orderToExecReportListPtr.first << std::endl;
            for ( auto& execReport : *(orderToExecReportListPtr.second) )
            {
                execReport->m_Header.SenderCompID = orderMassStatusRequestPtr->m_Header.TargetCompID;
                execReport->m_Header.TargetCompID = orderMassStatusRequestPtr->m_Header.SenderCompID;
                execReport->m_Header.TargetSubID = CORBA::string_dup(username.c_str());
            
                int ret = m_execution_report_dw->write( *(execReport), NULL );
            
                if (ret != DDS::RETCODE_OK)
                {
                    ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: MassOrderStatusRequestDataReader/Execution Report write returned %d.\n"), ret));
                    return false;
                }
            }
        }
    }
    
    return true;
}


} /* namespace DistributedATS */
