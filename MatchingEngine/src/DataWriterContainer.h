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

#ifndef DataWriterContainer_h
#define DataWriterContainer_h

#include <ace/Singleton.h>

#include <ace/Guard_T.h>
#include <ace/RW_Mutex.h>
#include <map>
#include <memory>

#include <LogonTypeSupportImpl.h>
#include <MarketDataIncrementalRefreshTypeSupportImpl.h>
#include <MarketDataRequestTypeSupportImpl.h>
#include <NewOrderSingleTypeSupportImpl.h>
#include <OrderCancelRequestTypeSupportImpl.h>
#include <OrderMassCancelRequestTypeSupportImpl.h>
#include <OrderMassStatusRequestAdapter.hpp>
#include <OrderMassCancelReportAdapter.hpp>
#include <SecurityListRequestTypeSupportImpl.h>
#include <OrderCancelReplaceRequestTypeSupportImpl.h>
#include <dds/DdsDcpsPublicationC.h>

#include <BasicDomainParticipant.h>
#include <Common.h>

namespace DistributedATS {

struct DataWriterContainer
{
    void setExecutionReportDataWriter(
      DistributedATS_ExecutionReport::ExecutionReportDataWriter_var &execution_report_dw) {
      _execution_report_dw = execution_report_dw;
    }

    void setMarketDataIncrementalRefreshDataWriter(
          DistributedATS_MarketDataIncrementalRefresh::
            MarketDataIncrementalRefreshDataWriter_var
                &marketdata_incremental_refresh_dw) {
      _marketdata_incremental_refresh_dw = marketdata_incremental_refresh_dw;
    }

    void setOrderCancelRejectDataWriter(
                                        DistributedATS_OrderCancelReject::OrderCancelRejectDataWriter_var
            &order_cancel_reject_dw) {
      _order_cancel_reject_dw = order_cancel_reject_dw;
    }

    void setOrderMassCancelReportDataWriter(
          DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataWriter_var
            &order_mass_cancel_report_dw) {
      _order_mass_cancel_report_dw = order_mass_cancel_report_dw;
    }

    void setSecurityListRequestDataWriter(
      DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_var
            &security_list_request_dw) {
      _security_list_request_dw = security_list_request_dw;
    }

    void setMarketDataRequestDataWriter(
          DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_var
            &market_date_request_dw) {
      _market_date_request_dw = market_date_request_dw;
    }
    
    DistributedATS_ExecutionReport::ExecutionReportDataWriter_var _execution_report_dw;
    DistributedATS_OrderCancelReject::OrderCancelRejectDataWriter_var
_order_cancel_reject_dw;
    DistributedATS_OrderMassCancelReport::OrderMassCancelReportDataWriter_var
_order_mass_cancel_report_dw;
    DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshDataWriter_var
_marketdata_incremental_refresh_dw;
    DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_var
_security_list_request_dw;
    DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_var
_market_date_request_dw;
};

typedef std::shared_ptr<DataWriterContainer> DataWriterContainerPtr;
};

#endif DataWriterContainer_h
