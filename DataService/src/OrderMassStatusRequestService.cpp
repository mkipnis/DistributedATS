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

#include "OrderMassStatusRequestService.h"
#include <OrderMassStatusRequestPubSubTypes.hpp>
#include <ExecutionReportPubSubTypes.hpp>
#include "ExecutionReportDataReaderListenerImpl.h"
#include <Common.h>

#include <thread>
#include <chrono>

namespace DistributedATS {

OrderMassStatusRequestService::OrderMassStatusRequestService( std::shared_ptr<distributed_ats_utils::basic_domain_participant>
                            basic_domain_participant_ptr )
{
    _basic_domain_participant_ptr = basic_domain_participant_ptr;
    m_executionReports = std::make_shared<ExecutionReports>( ExecutionReports() );
    _orderMassStatusRequestQueuePtr = std::make_shared<OrderMassStatusRequestQueue>();
    
    std::atomic_init(&_is_running, true);
    _service_thread = std::thread(&OrderMassStatusRequestService::service, this);
}

OrderMassStatusRequestService::~OrderMassStatusRequestService()
{
    std::atomic_init(&_is_running, false);
    _service_thread.join();
}

void OrderMassStatusRequestService::createOrderMassStatusRequestListener()
{
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest orderMassStatusRequest;
    
    _order_mass_status_request_topic_tuple = _basic_domain_participant_ptr->make_topic<
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestPubSubType,
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest>
        ( ORDER_MASS_STATUS_REQUEST_TOPIC_NAME );
    
    std::string data_service_filter_expression_exec_report = "DATS_Destination = %0 and DATS_DestinationUser = %1";
    
    
    _order_mass_status_request_data_reader_tuple =
        _basic_domain_participant_ptr->make_data_reader_tuple(_order_mass_status_request_topic_tuple,
            new DistributedATS::OrderMassStatusRequestDataReaderListenerImpl( _orderMassStatusRequestQueuePtr ),  "MASS_STATUS_REQUEST_FILTER", data_service_filter_expression_exec_report,
                                    { "DATA_SERVICE", _basic_domain_participant_ptr->get_participant_name()});
    

}

void OrderMassStatusRequestService::createExecutionReportListener()
{
    DistributedATS_ExecutionReport::ExecutionReport executionReport;
     
    _execution_report_topic_tuple = _basic_domain_participant_ptr->make_topic
        < DistributedATS_ExecutionReport::ExecutionReportPubSubType,
            DistributedATS_ExecutionReport::ExecutionReport>
        ( EXECUTION_REPORT_TOPIC_NAME );
    
    std::string data_service_filter_expression_exec_report = "DATS_Source = %0 and DATS_SourceUser = %1";
    
    _execution_report_data_reader_tuple = _basic_domain_participant_ptr->make_data_reader_tuple(_execution_report_topic_tuple,
                    new DistributedATS::ExecutionReportDataReaderListenerImpl(m_executionReports),
                    "EXECUTION_REPORT_FILTER", data_service_filter_expression_exec_report,
                        { "MATCHING_ENGINE", _basic_domain_participant_ptr->get_participant_name()});
    
    
    _execution_report_dw = _basic_domain_participant_ptr->make_data_writer( _execution_report_topic_tuple );
    
}

int OrderMassStatusRequestService::service()
{
    
    while(_is_running)
    {
        OrderMassStatusRequestPtr  order_mass_status_request;
        
        if ( _orderMassStatusRequestQueuePtr->pop( order_mass_status_request ) )
        {
            processMassOrderStatusServiceRequest(order_mass_status_request);

        } else {
            std::this_thread::sleep_for(std::chrono::duration<long double, std::milli>(1000));
        }
    }
    
    return 0;

}


OrderToExecutionReportMapPtr OrderMassStatusRequestService::getOrderToExecutionReportMap( std::string& senderCompId )
{
	auto orderIdToExecutionReportMap = m_executionReports->find(senderCompId);

	if ( orderIdToExecutionReportMap != m_executionReports->end() )
		return orderIdToExecutionReportMap->second;

	return OrderToExecutionReportMapPtr();
}
    
bool OrderMassStatusRequestService::processMassOrderStatusServiceRequest( OrderMassStatusRequestPtr&  orderMassStatusRequestPtr )
{
    
    LOG4CXX_INFO(logger, "Received Mass Order Status Request : [" <<  orderMassStatusRequestPtr->DATS_Source() << ":" <<  orderMassStatusRequestPtr->DATS_Destination() << "]");
    
    auto username = orderMassStatusRequestPtr->DATS_SourceUser();
    
    OrderToExecutionReportMapPtr orderToExecutionReportMapPtr =
    getOrderToExecutionReportMap( username );
    
    if ( orderToExecutionReportMapPtr )
    {
        for ( auto& orderToExecReportListPtr : *orderToExecutionReportMapPtr )
        {
            std::cout << "Need to publish execution report for the following order id : " << orderToExecReportListPtr.first << std::endl;
            for ( auto& execReport : *(orderToExecReportListPtr.second) )
            {
                execReport->DATS_Source(orderMassStatusRequestPtr->DATS_Destination());
                execReport->DATS_Destination(orderMassStatusRequestPtr->DATS_Source());
                execReport->DATS_DestinationUser(username);
            
                int ret = _execution_report_dw->write( execReport.get() );
                
                if ( ret != eprosima::fastdds::dds::RETCODE_OK ) {
                    LOG4CXX_ERROR(logger, "MassOrderStatusRequestDataReader/Execution Report write returned :" << ret);
                }
            
            }
        }
    }
    
    return true;
}


} /* namespace DistributedATS */
