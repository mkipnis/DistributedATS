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

#include <LogonPubSubTypes.h>
#include <LogoutPubSubTypes.h>
#include <MarketDataIncrementalRefreshPubSubTypes.h>
#include <MarketDataSnapshotFullRefreshPubSubTypes.h>
#include <ExecutionReportPubSubTypes.h>
#include <OrderCancelRejectPubSubTypes.h>
#include <OrderMassCancelReportPubSubTypes.h>
#include <SecurityListPubSubTypes.h>
#include <SecurityList.h>



namespace DistributedATS {

struct DataReaderContrainer {
    
    
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_Logon::Logon> _logon_data_reader_tuple;

    distributed_ats_utils::topic_tuple_ptr<DistributedATS_Logout::Logout> _logout_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_Logout::Logout> _logout_data_reader_tuple;

    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh> _market_data_incremental_refresh_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh> _market_data_incremental_refresh_data_reader_tuple;
    
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh> _market_data_snapshot_full_refresh_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh> _market_data_snapshot_full_refresh_data_reader_tuple;


    distributed_ats_utils::topic_tuple_ptr<DistributedATS_ExecutionReport::ExecutionReport> _execution_report_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_ExecutionReport::ExecutionReport>  _execution_report_data_reader_tuple;
        
    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderCancelReject::OrderCancelReject> _order_cancel_reject_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_OrderCancelReject::OrderCancelReject>  _order_cancel_reject_data_reader_tuple;

    distributed_ats_utils::topic_tuple_ptr<DistributedATS_OrderMassCancelReport::OrderMassCancelReport> _order_mass_cancel_report_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_OrderMassCancelReport::OrderMassCancelReport>  _order_mass_cancel_report_data_reader_tuple;

    distributed_ats_utils::topic_tuple_ptr<DistributedATS_SecurityList::SecurityList> _security_list_topic_tuple;
    distributed_ats_utils::data_reader_tuple_ptr<DistributedATS_SecurityList::SecurityList>  _security_list_data_reader_tuple;
    
    
    ~DataReaderContrainer()
    {
        std::cout << "~DataReaderContrainer" << std::endl;
    }

  DataReaderContrainer(
      distributed_ats_utils::basic_domain_participant_ptr participant_ptr,
      DistributedATS::DATSApplication &application,
      std::string targetCompIdFilter) {
      
    //
    // Data Reader Topic
    //

      auto& logon_topic_tuple = application.get_data_writer_container()->_logon_topic_tuple;
    
      std::string authCompIdFilter = "header.SenderCompID='AUTH'";
      
      _logon_data_reader_tuple = participant_ptr->make_data_reader_tuple(logon_topic_tuple,
                        new DistributedATS::LogonDataReaderListenerImpl(application), "FILTERED_LOGON", authCompIdFilter);
      


      _logout_topic_tuple = participant_ptr->make_topic<
      DistributedATS_Logout::LogoutPubSubType,
      DistributedATS_Logout::Logout>(
        LOGOUT_TOPIC_NAME);
      
      _logout_data_reader_tuple = participant_ptr->make_data_reader_tuple(_logout_topic_tuple,
                        new DistributedATS::LogoutDataReaderListenerImpl(application),"FILTERED_LOGOUT", authCompIdFilter);


      _security_list_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_SecurityList::SecurityListPubSubType,
      DistributedATS_SecurityList::SecurityList>(
               SECURITY_LIST_TOPIC_NAME);
      
      
      
      _security_list_data_reader_tuple = participant_ptr->make_data_reader_tuple(_security_list_topic_tuple,
                                                                                 new DistributedATS::SecurityListDataReaderListenerImpl(application)); //,"FILTERED_REF_DATA", targetCompIdFilter);
      

      _market_data_incremental_refresh_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefreshPubSubType,
      DistributedATS_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh>(
            MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);
      
      _market_data_incremental_refresh_data_reader_tuple = participant_ptr->make_data_reader_tuple(_market_data_incremental_refresh_topic_tuple,
                                new DistributedATS::MarketDataIncrementalRefreshDataReaderListenerImpl(application));

      
      
      _market_data_snapshot_full_refresh_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshPubSubType,
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefresh>(
            MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME);
      
      _market_data_snapshot_full_refresh_data_reader_tuple =
      participant_ptr->make_data_reader_tuple(_market_data_snapshot_full_refresh_topic_tuple,
                new DistributedATS::MarketDataSnapshotFullRefreshDataReaderListenerImpl(application));

      
      _execution_report_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_ExecutionReport::ExecutionReportPubSubType,
      DistributedATS_ExecutionReport::ExecutionReport>(
            EXECUTION_REPORT_TOPIC_NAME);


       
      _execution_report_data_reader_tuple = participant_ptr->make_data_reader_tuple(_execution_report_topic_tuple,
                                        new DistributedATS::ExecutionReportDataReaderListenerImpl(application),
                                            "FILTERED_EXEC_REPORT", targetCompIdFilter);
      
      _order_cancel_reject_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_OrderCancelReject::OrderCancelRejectPubSubType,
      DistributedATS_OrderCancelReject::OrderCancelReject>(
            ORDER_CANCEL_REJECT_TOPIC_NAME);
      
      _order_cancel_reject_data_reader_tuple = participant_ptr->make_data_reader_tuple(_order_cancel_reject_topic_tuple,
                        new DistributedATS::OrderCancelRejectDataReaderListenerImpl(application),
                        "FILTERED_ORDER_CANCEL_REJECT", targetCompIdFilter);
    

      _order_mass_cancel_report_topic_tuple =
      participant_ptr->make_topic<
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportPubSubType,
      DistributedATS_OrderMassCancelReport::OrderMassCancelReport>(
            ORDER_MASS_CANCEL_REPORT_TOPIC_NAME);
      
      _order_mass_cancel_report_data_reader_tuple = participant_ptr->make_data_reader_tuple(_order_mass_cancel_report_topic_tuple,
                        new DistributedATS::OrderMassCancelReportDataReaderListenerImpl(application),
                            "FILTERED_ORDER_MASS_CANCEL", targetCompIdFilter);
  };
};
}; // namespace DistributedATS

