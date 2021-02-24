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


#ifndef DATS_APPLICATION_H
#define DATS_APPLICATION_H

#include <map>
#include <memory>
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Queue.h>
#include <quickfix/SessionFactory.h>
#include <quickfix/fix44/MarketDataRequest.h>
#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/OrderCancelRequest.h>
#include <quickfix/fix44/OrderMassCancelRequest.h>
#include <quickfix/fix44/OrderMassStatusRequest.h>
#include <quickfix/fix44/SecurityListRequest.h>
#include <quickfix/fix44/OrderCancelReplaceRequest.h>
#include <stdio.h>

#include "AuthServiceHelper.h"
#include <ace/Message_Queue_T.h>

namespace DistributedATS {
class SocketConnection;
class SocketAcceptor;
class DataWriterContrainer;

class DATSApplication : public FIX::Application, public FIX::MessageCracker {

  // Application overloads
  void onCreate(const FIX::SessionID &);
  void onLogon(const FIX::SessionID &sessionID);
  void onLogout(const FIX::SessionID &sessionID);
  void toAdmin(FIX::Message &, const FIX::SessionID &);
  void toApp(FIX::Message &, const FIX::SessionID &) throw(FIX::DoNotSend);
  void fromAdmin(const FIX::Message &,
                 const FIX::SessionID &) throw(FIX::FieldNotFound,
                                               FIX::IncorrectDataFormat,
                                               FIX::IncorrectTagValue,
                                               FIX::RejectLogon);
  void
  fromApp(const FIX::Message &message,
          const FIX::SessionID &sessionID) throw(FIX::FieldNotFound,
                                                 FIX::IncorrectDataFormat,
                                                 FIX::IncorrectTagValue,
                                                 FIX::UnsupportedMessageType);

  void onMessage(const FIX44::NewOrderSingle &, const FIX::SessionID &);
  void onMessage(const FIX44::OrderCancelRequest &, const FIX::SessionID &);
  void onMessage(const FIX44::OrderMassCancelRequest &, const FIX::SessionID &);
  void onMessage(const FIX44::SecurityListRequest &, const FIX::SessionID &);
  void onMessage(const FIX44::MarketDataRequest &, const FIX::SessionID &);
  void onMessage(const FIX44::OrderMassStatusRequest &, const FIX::SessionID &);
  void onMessage(const FIX44::OrderCancelReplaceRequest &, const FIX::SessionID &);


public:
  DATSApplication(
      std::string dataService, std::string senderCompId,
      std::shared_ptr<DistributedATS::DataWriterContrainer> dataWriterContainer)
      : _connection_id(0), _dataService(dataService),
        _senderCompId(senderCompId),
        _dataWriterContainer(dataWriterContainer){};

  // publishes FIX Message from DDS data readers;
  static void publishToClient(FIX::Message &message);
  static void broadcastToClients(FIX::Message &message);

  void onDisconnect(const FIX::SessionID &sessionID,
                    SocketConnection *pSocketConnection);

  template <class ADAPTER, class DATA_WRITER, class DATA, class LOGGER>
  void publishToDDS(const FIX::Message &message, DATA_WRITER &dataWriter,
                    const std::string &sender, const std::string &target,
                    const std::string &senderSubID);

  bool insertPendingLogonSocketConnection(const FIX::Message &message,
                                          SocketConnection *);
  void processDDSLogon(FIX::Message &message);
  void processDDSLogout(std::string &connectionToken, FIX::Message &message);

  std::string getConnectionToken(
      const FIX::Message
          &); // RawData of Login Message, it will be used to cross reference
              // logon reply with Socket Connection

public:
  void setAuthService(std::shared_ptr<AuthServiceHelper> &authService) {
    _authService = authService;
  };

private:
  DistributedATS::SocketAcceptor *_acceptor;
  std::shared_ptr<AuthServiceHelper> _authService;
  std::shared_ptr<DistributedATS::DataWriterContrainer> _dataWriterContainer;

  unsigned int _connection_id; // used in token generation
  std::string _dataService;
  std::string _senderCompId;

  // Must lock whenever we interract with QuickFIX Sessions(i.e. if quickfix client disconnets and session state is update on QuickFIX Threads, we can't send messages from DDS threads in an non-synchronized matter )
    static FIX::Mutex s_quickFIXSessionStateMutex;
};
}; // namespace DistributedATS

#endif /* DATS_APPLICATION_H */
