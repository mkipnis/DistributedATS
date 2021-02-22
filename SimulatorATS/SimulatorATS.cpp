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

#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketInitiator.h"
#ifdef HAVE_SSL
#include "quickfix/ThreadedSSLSocketInitiator.h"
#include "quickfix/SSLSocketInitiator.h"
#endif
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>

#include "Market.hpp"
#include "OrderManager.hpp"

int main( int argc, char** argv )
{
  if ( argc < 2 )
  {
    std::cout << "usage: " << argv[ 0 ]
    << " config_file " << std::endl;
    return 0;
  }

  std::string config_file = argv[ 1 ];

  FIX::Initiator * initiator = 0;

  try
  {
      FIX::SessionSettings settings( config_file );

      SimulatorATS::OrderManagerPtr orderManagerPtr = std::make_shared< SimulatorATS::OrderManager >();
      SimulatorATS::MarketPtr marketPtr = std::make_shared< SimulatorATS::Market >( orderManagerPtr );
      
      SimulatorATS::Application application( settings, marketPtr, orderManagerPtr );

      FIX::FileStoreFactory storeFactory( settings );
      FIX::FileLogFactory logFactory( settings );

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
