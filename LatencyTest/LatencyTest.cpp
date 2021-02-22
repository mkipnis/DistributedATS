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

#include <NewOrderSingleTypeSupportImpl.h>
#include "NewOrderSingleDataReaderListenerImpl.h"
#include "ExecutionReportDataReaderListenerImpl.h"

#include <ace/Get_Opt.h>
#include <Common.h>

#include "quickfix/FileStore.h"
#include "quickfix/NullStore.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include <BasicDomainParticipant.h>

#include "LatencyTest.h"


int main( int argc, char** argv )
{
  if ( argc < 2 )
  {
    std::cout << "usage: " << argv[ 0 ]
    << "  -c LatencyTest.cfg  -DCPSConfigFile $DATS_HOME/Protocols/tcp.ini" << std::endl;
    return 0;
  }
  std::string file = argv[ 1 ];

  FIX::Initiator * initiator = 0;

  DDS::DomainParticipantFactory_var dpf = DDS::DomainParticipantFactory::_nil();

  try
  {
      LatencyTest::LatencyStatsPtr latencyStatsPtr( std::make_shared<LatencyTest::latency_stats>() );

    dpf = TheParticipantFactoryWithArgs(argc, argv);

    ACE_Get_Opt cmd_opts( argc, argv, ":c:" );

    int option;

    std::string quickfix_config_file = "";

    while ( (option = cmd_opts()) != EOF )
    {
      switch( option )
      {
        case 'c' :
          quickfix_config_file = cmd_opts.opt_arg();
        break;
      }
    }

    distributed_ats_utils::BasicDomainParticipant participant( dpf, DISTRIBUTED_ATS_DOMAIN_ID );
    participant.createSubscriber();


    DDS::Topic_var new_order_single_topic = participant.createTopicAndRegisterType
                < DistributedATS_NewOrderSingle::NewOrderSingleTypeSupport_var,
      DistributedATS_NewOrderSingle::NewOrderSingleTypeSupportImpl > ( NEW_ORDER_SINGLE_TOPIC_NAME );

    DDS::DataReaderListener_var newOrderSingleDataListener(new LatencyTest::NewOrderSingleDataReaderListenerImpl( latencyStatsPtr ));

    participant.createDataReaderListener( new_order_single_topic, newOrderSingleDataListener );

    DDS::Topic_var execution_report_topic =
                         participant.createTopicAndRegisterType < DistributedATS_ExecutionReport::ExecutionReportTypeSupport_var,
      DistributedATS_ExecutionReport::ExecutionReportTypeSupportImpl >
                        ( EXECUTION_REPORT_TOPIC_NAME );

    DDS::DataReaderListener_var executionReportDataListener(new LatencyTest::ExecutionReportDataReaderListenerImpl( latencyStatsPtr  ));

    participant.createDataReaderListener( execution_report_topic, executionReportDataListener );

    FIX::SessionSettings settings( quickfix_config_file );

    LatencyTest::Application application( settings, latencyStatsPtr );
    FIX::NullStoreFactory storeFactory;
    FIX::ScreenLogFactory logFactory( settings );

    initiator = new FIX::SocketInitiator( application, storeFactory, settings, logFactory );

    initiator->start();
    application.run();
    initiator->stop();
    delete initiator;

    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
    delete initiator;
    return 1;
  }
}
