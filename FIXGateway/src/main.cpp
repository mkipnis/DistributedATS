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

#include <LogonAdapter.hpp>
#include <LogonTypeSupportImpl.h>
#include <LogoutAdapter.hpp>
#include <LogoutTypeSupportImpl.h>

#include <NewOrderSingleAdapter.hpp>
#include <NewOrderSingleTypeSupportImpl.h>

#include <OrderCancelRejectAdapter.hpp>
#include <OrderCancelRejectTypeSupportImpl.h>
#include <OrderCancelRequestAdapter.hpp>
#include <OrderCancelRequestTypeSupportImpl.h>

#include <MarketDataRequestTypeSupportImpl.h>
#include <OrderMassCancelReportAdapter.hpp>
#include <OrderMassCancelReportTypeSupportImpl.h>
#include <OrderMassCancelRequestAdapter.hpp>
#include <OrderMassCancelRequestTypeSupportImpl.h>
#include <SecurityListRequestTypeSupportImpl.h>
#include <SecurityListTypeSupportImpl.h>

#include <BasicDomainParticipant.h>

#include "quickfix/FileStore.h"
#include "quickfix/Log.h"
#include "quickfix/NullStore.h"
#include "quickfix/SessionSettings.h"
#include <fstream>
#include <iostream>
#include <quickfix/SessionFactory.h>
#include <string>

#include "SocketAcceptor.h"
#include "SocketConnection.h"

#include <BasicDomainParticipant.h>
#include <Common.h>
#include <ace/Get_Opt.h>

#include "ExecutionReportDataReaderListenerImpl.h"
#include "LogonDataReaderListenerImpl.hpp"
#include "LogoutDataReaderListenerImpl.hpp"
#include "OrderCancelRejectDataReaderListenerImpl.h"
#include "OrderMassCancelReportDataReaderListenerImpl.h"
#include "SecurityListDataReaderListenerImpl.h"

#include "MarketDataIncrementalRefreshDataReaderListenerImpl.h"
#include "MarketDataSnapshotFullRefreshDataReaderListenerImpl.h"

#include "DataReaderContainer.h"
#include "DataWriterContainer.hpp"

#include "Application.hpp"
#include "AuthServiceHelper.h"

#include <fstream>

#include "ace/streams.h"
#include <ace/Message_Queue_T.h>

#include "FileLog.h"
#include <ace/Log_Msg.h>
#include <ace/Log_Msg_Callback.h>
#include <ace/Log_Record.h>
#include <ace/streams.h>

#include <atomic>

std::atomic<bool> is_running;

namespace DistributedATS {

class SignalHandler : public ACE_Event_Handler {
public:
  virtual int handle_signal(int signum, siginfo_t * = 0, ucontext_t * = 0) {
    std::cout << "Lets hangle signal : " << signum << std::endl;
    is_running.store(false);
    return 0;
  };
};

}; // namespace DistributedATS

int main(int argc, char **argv) {
  DDS::DomainParticipantFactory_var dpf = DDS::DomainParticipantFactory::_nil();

  if (argc < 2) {
    std::cout << "usage: " << argv[0] << std::endl
              << "\t\t-c quickfix-config-file" << std::endl;

    return 0;
  }

  std::string quickfix_config_file = "";
  std::string sender_comp_id = "";
  std::string data_service_name = "";

  try {

    dpf = TheParticipantFactoryWithArgs(argc, argv);

    distributed_ats_utils::BasicDomainParticipantPtr participant_ptr =
        std::make_shared<distributed_ats_utils::BasicDomainParticipant>(
            dpf, DISTRIBUTED_ATS_DOMAIN_ID);

    participant_ptr->createSubscriber();
    participant_ptr->createPublisher();

    ACE_Get_Opt cmd_opts(argc, argv, ":c:");

    int option;

    while ((option = cmd_opts()) != EOF) {
      switch (option) {
      case 'c':
        quickfix_config_file = cmd_opts.opt_arg();
        break;
      }
    }

    if (quickfix_config_file.empty()) {
      std::cerr << "Error: Config file name is not specified." << std::endl;
      return -1;
    }

    is_running = true;

    auto settings =
        std::make_shared<FIX::SessionSettings>(quickfix_config_file);

    FIX::SessionID default_session_id("FIX.4.4", "DEFAULT", "DEFAULT");
    const FIX::Dictionary *default_dictionary_tmp =
        &settings->get(default_session_id);

    FIX::Dictionary *default_dictionary = new FIX::Dictionary();
    default_dictionary = const_cast<FIX::Dictionary *>(default_dictionary_tmp);

    sender_comp_id = settings->get().getString("SenderCompID");
    data_service_name = default_dictionary->getString("DataService");

    std::string file_log_path = settings->get().getString("FileLogPath");
    std::string file_store_path = settings->get().getString("FileStorePath");

    std::string dds_log_file_name_prefix =
        file_log_path + "/" + sender_comp_id + "." + data_service_name;

    //
    // Lets deal with signals
    //
    signal(SIGPIPE, SIG_IGN);

    ACE_Sig_Handler handler;
    DistributedATS::SignalHandler signalHandler;
    // add signal handler for the SIGINT signal here
    ACE_Sig_Handler sig_handler;
    sig_handler.register_handler(SIGINT, &signalHandler);

    // Separate log files for DDS in and out messages to avoid serialization of
    // threads
    std::string fix_prefix = sender_comp_id + "." + data_service_name;
    std::string dds_output_stream_log_file =
        dds_log_file_name_prefix + ".dds.fix.log";

    std::cout << "DDS out filename : " << dds_output_stream_log_file
              << std::endl;

    std::ofstream *dds_output_stream =
        new std::ofstream(dds_output_stream_log_file, ios::app);

    if (dds_output_stream->bad()) {
      delete dds_output_stream;
    } else {
      ACE_LOG_MSG->msg_ostream(dds_output_stream, true);
    }

    ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
    ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM);

    std::string dds_input_stream_log_file =
        dds_log_file_name_prefix + ".fix.dds.log";

    std::cout << "DDS input filename : " << dds_input_stream_log_file
              << std::endl;

    std::ofstream *dds_input_stream =
        new std::ofstream(dds_input_stream_log_file, ios::app);

    auto data_writer_container =
        std::make_shared<DistributedATS::DataWriterContrainer>(participant_ptr);

    ACE_DEBUG((LM_INFO,
               ACE_TEXT("(%P|%t) SenderCompID : [%s] | DataService : [%s]"),
               sender_comp_id.c_str(), data_service_name.c_str()));

    std::string target_comp_id_filter =
        "m_Header.TargetCompID='" + sender_comp_id +
        "'"; // content filtering for messages directed to instances of gateways
             // with given sender comp id.  These messages include Logon/Logout,
             // ExectionReports, MassStatus, etc.
    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) TargetCompID filter : [%s]"),
               target_comp_id_filter.c_str()));

    FIX::FileStoreFactory store_factory(file_store_path);
    // FIX::NullStoreFactory store_factory;
    DistributedATS::FileLogFactory log_factory(*settings, fix_prefix);

    DistributedATS::DATSApplication application(
        data_service_name, sender_comp_id, data_writer_container);

    auto acceptor = std::make_shared<DistributedATS::SocketAcceptor>(
        application, store_factory, *settings, log_factory, dds_input_stream);
    auto sessionFactory = std::make_shared<FIX::SessionFactory>(
        application, store_factory, &log_factory);

    auto data_reader_container =
        std::make_shared<DistributedATS::DataReaderContrainer>(
            participant_ptr, application, target_comp_id_filter);
    auto authService = std::make_shared<AuthServiceHelper>(
        settings, sessionFactory, default_dictionary, sender_comp_id);

    application.setAuthService(authService);

    acceptor->start();

    while (is_running == true)
      ACE_OS::sleep(1);

    acceptor->stop();

    delete default_dictionary;
    delete dds_output_stream;
    delete dds_input_stream;

    return 0;
  } catch (std::exception &e) {

    ACE_ERROR ((LM_CRITICAL, ACE_TEXT("(%P|%t|%D) ERROR: Exception during the initialization of FIX Gateway : %s\n"), e.what()));

    std::cout << e.what() << std::endl;
    return 1;
  }
}
