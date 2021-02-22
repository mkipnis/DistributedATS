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


#ifndef MASSORDERSTATUSSERVICE_H_
#define MASSORDERSTATUSSERVICE_H_

#include <memory>
#include <map>
#include <OrderMassStatusRequestTypeSupportImpl.h>
#include <ExecutionReportTypeSupportImpl.h>

#include <BasicDomainParticipant.h>

typedef std::shared_ptr<DistributedATS_ExecutionReport::ExecutionReport> ExecutionReportPtr;
typedef std::list<ExecutionReportPtr> ExecutionReportPtrList;
typedef std::shared_ptr<ExecutionReportPtrList> ExecutionReportPtrListPtr;
typedef std::map<std::string, ExecutionReportPtrListPtr > OrderToExecutionReportMap;
typedef std::shared_ptr<OrderToExecutionReportMap> OrderToExecutionReportMapPtr;
typedef std::map<std::string, OrderToExecutionReportMapPtr> ExecutionReports;
typedef std::shared_ptr<ExecutionReports> ExecutionReportsPtr;

namespace DistributedATS {


class MassOrderStatusService : public ACE_Task <ACE_MT_SYNCH>
{
public:
    MassOrderStatusService( std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> basicDomainParticipantPtr,
    						ACE_Thread_Manager *thr_mgr );
	virtual ~MassOrderStatusService();
    
    void createOrderMassStatusRequestListener( const std::string& data_service_filter_expression );
    void createExecutionReportListener( const std::string& data_service_filter_expression );
    
	OrderToExecutionReportMapPtr getOrderToExecutionReportMap( std::string& senderCompId );
    bool processMassOrderStatusServiceRequest( DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest* orderMassStatusRequestPtr );

    virtual int svc (void);

private:
	ExecutionReportsPtr m_executionReports;
    
    DistributedATS_ExecutionReport::ExecutionReportDataWriter_var m_execution_report_dw;
    
    std::shared_ptr<distributed_ats_utils::BasicDomainParticipant> m_basicDomainParticipantPtr;    
};

} /* namespace DistributedATS */

#endif /* MASSORDERSTATUSSERVICE_H_ */
