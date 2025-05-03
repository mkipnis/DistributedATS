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


#pragma once

#include <memory>
#include <map>
#include <thread>

#include <BasicDomainParticipant.h>

#include "OrderMassStatusRequestDataReaderListenerImpl.h"

#include <ExecutionReport.hpp>

typedef std::shared_ptr<DistributedATS_ExecutionReport::ExecutionReport> ExecutionReportPtr;
typedef std::list<ExecutionReportPtr> ExecutionReportPtrList;
typedef std::shared_ptr<ExecutionReportPtrList> ExecutionReportPtrListPtr;
typedef std::map<std::string, ExecutionReportPtrListPtr > OrderToExecutionReportMap;
typedef std::shared_ptr<OrderToExecutionReportMap> OrderToExecutionReportMapPtr;
typedef std::map<std::string, OrderToExecutionReportMapPtr> ExecutionReports;
typedef std::shared_ptr<ExecutionReports> ExecutionReportsPtr;


namespace DistributedATS {


class OrderMassStatusRequestService
{
public:
    OrderMassStatusRequestService( std::shared_ptr<distributed_ats_utils::basic_domain_participant> );
    virtual ~OrderMassStatusRequestService();
    
    void createOrderMassStatusRequestListener();
    void createExecutionReportListener();
    
    OrderToExecutionReportMapPtr getOrderToExecutionReportMap( std::string& senderCompId );
    bool processMassOrderStatusServiceRequest( OrderMassStatusRequestPtr& orderMassStatusRequestPtr );
    
    int service();
    
private:
    ExecutionReportsPtr m_executionReports;
    
    distributed_ats_utils::data_writer_ptr _execution_report_dw;
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_ExecutionReport::ExecutionReport> _execution_report_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_ExecutionReport::ExecutionReport> _execution_report_data_reader_tuple;

    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest> _order_mass_status_request_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest> _order_mass_status_request_data_reader_tuple;

    
    std::shared_ptr<distributed_ats_utils::basic_domain_participant> _basic_domain_participant_ptr;
    
    OrderMassStatusRequestQueuePtr _orderMassStatusRequestQueuePtr;
    
    std::thread _service_thread;
    
    std::atomic<bool> _is_running;

};

} /* namespace DistributedATS */
