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

#ifndef DataReaderContainer_h
#define DataReaderContainer_h

namespace DistributedATS {

struct DataReaderContrainer {
  DataReaderContrainer(
      distributed_ats_utils::BasicDomainParticipantPtr participantPtr,
      DistributedATS::DATSApplication &application,
      std::string targetCompIdFilter) {
    //
    // Data Reader Topic
    //
    DDS::Topic_var logon_topic = participantPtr->createTopicAndRegisterType<
      DistributedATS_Logon::LogonTypeSupport_var, DistributedATS_Logon::LogonTypeSupportImpl>(
        LOGON_TOPIC_NAME);

    DDS::Topic_var logout_topic = participantPtr->createTopicAndRegisterType<
      DistributedATS_Logout::LogoutTypeSupport_var, DistributedATS_Logout::LogoutTypeSupportImpl>(
        LOGOUT_TOPIC_NAME);

    DDS::Topic_var market_data_incremental_refresh_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshTypeSupport_var,
      DistributedATS_MarketDataIncrementalRefresh::
                MarketDataIncrementalRefreshTypeSupportImpl>(
            MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);

    DDS::Topic_var market_data_snapshot_full_refresh_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshTypeSupport_var,
      DistributedATS_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshTypeSupportImpl>(
            MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME);

    DDS::Topic_var execution_report_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_ExecutionReport::ExecutionReportTypeSupport_var,
      DistributedATS_ExecutionReport::ExecutionReportTypeSupportImpl>(
            EXECUTION_REPORT_TOPIC_NAME);

    DDS::Topic_var order_cancel_reject_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_OrderCancelReject::OrderCancelRejectTypeSupport_var,
      DistributedATS_OrderCancelReject::OrderCancelRejectTypeSupportImpl>(
            ORDER_CANCEL_REJECT_TOPIC_NAME);

    DDS::Topic_var order_mass_cancel_report_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportTypeSupport_var,
      DistributedATS_OrderMassCancelReport::OrderMassCancelReportTypeSupportImpl>(
            ORDER_MASS_CANCEL_REPORT_TOPIC_NAME);

    DDS::Topic_var security_list_topic =
        participantPtr->createTopicAndRegisterType<
      DistributedATS_SecurityList::SecurityListTypeSupport_var,
      DistributedATS_SecurityList::SecurityListTypeSupportImpl>(
            SECURITY_LIST_TOPIC_NAME);

    //
    // Content Filters
    //

    std::string authCompIdFilter = "m_Header.SenderCompID='AUTH'";

    DDS::ContentFilteredTopic_ptr cft_logon =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_LOGON", logon_topic, authCompIdFilter.c_str(),
            DDS::StringSeq());

    DDS::ContentFilteredTopic_ptr cft_logout =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_LOGOUT", logout_topic, authCompIdFilter.c_str(),
            DDS::StringSeq());

    DDS::ContentFilteredTopic_ptr cft_ref_data =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_REF_DATA", security_list_topic,
            targetCompIdFilter.c_str(), DDS::StringSeq());

    DDS::ContentFilteredTopic_ptr cft_exec_report =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_EXEC_REPORT", execution_report_topic,
            targetCompIdFilter.c_str(), DDS::StringSeq());

    DDS::ContentFilteredTopic_ptr cft_mdc_full_refresh =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_MDC_FULL_REFRESH",
            market_data_snapshot_full_refresh_topic, targetCompIdFilter.c_str(),
            DDS::StringSeq());

    DDS::ContentFilteredTopic_ptr cft_order_cancel_reject =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_ORDER_CANCEL_REJECT", order_cancel_reject_topic,
            targetCompIdFilter.c_str(), DDS::StringSeq());

    DDS::ContentFilteredTopic_ptr cft_order_mass_cancel_report =
        participantPtr->getDomainParticipant()->create_contentfilteredtopic(
            "FILTERED_ORDER_MASS_CANCEL", order_mass_cancel_report_topic,
            targetCompIdFilter.c_str(), DDS::StringSeq());

    //
    // Data Listeners
    //
    DDS::DataReaderListener_var logonDataListener(
        new DistributedATS::LogonDataReaderListenerImpl(application));
    DDS::DataReaderListener_var logoutDataListener(
        new DistributedATS::LogoutDataReaderListenerImpl(application));
    DDS::DataReaderListener_var executionReportDataListener(
        new DistributedATS::ExecutionReportDataReaderListenerImpl());
    DDS::DataReaderListener_var marketDataIncrementalRefreshtDataListener(
        new DistributedATS::
            MarketDataIncrementalRefreshDataReaderListenerImpl());
    DDS::DataReaderListener_var marketDataSnapdshotFullRefreshtDataListener(
        new DistributedATS::
            MarketDataSnapshotFullRefreshDataReaderListenerImpl());
    DDS::DataReaderListener_var orderCancelRejectDataListener(
        new DistributedATS::OrderCancelRejectDataReaderListenerImpl());
    DDS::DataReaderListener_var orderMassCancelReportDataListener(
        new DistributedATS::OrderMassCancelReportDataReaderListenerImpl());
    DDS::DataReaderListener_var securityListReportDataListener(
        new DistributedATS::SecurityListDataReaderListenerImpl());

    participantPtr->createDataReaderListener(cft_logon, logonDataListener);
    participantPtr->createDataReaderListener(cft_logout, logoutDataListener);
    participantPtr->createDataReaderListener(cft_exec_report,
                                             executionReportDataListener);
    participantPtr->createDataReaderListener(
        market_data_incremental_refresh_topic,
        marketDataIncrementalRefreshtDataListener);
    participantPtr->createDataReaderListener(
        cft_mdc_full_refresh, marketDataSnapdshotFullRefreshtDataListener);
    participantPtr->createDataReaderListener(cft_order_cancel_reject,
                                             orderCancelRejectDataListener);
    participantPtr->createDataReaderListener(cft_order_mass_cancel_report,
                                             orderMassCancelReportDataListener);
    participantPtr->createDataReaderListener(cft_ref_data,
                                             securityListReportDataListener);
  };
};
}; // namespace DistributedATS

#endif /* DataReaderContainer_h */
