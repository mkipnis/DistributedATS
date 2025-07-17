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

#ifndef FIXGATEWAY_SOCKETACCEPTOR_H
#define FIXGATEWAY_SOCKETACCEPTOR_H


#include "Application.hpp"
#include "SocketConnection.h"
#include <quickfix/Acceptor.h>
#include <quickfix/SocketServer.h>

namespace DistributedATS {
// class Application;
/// Socket implementation of Acceptor.
class SocketAcceptor : public FIX::Acceptor, FIX::SocketServer::Strategy {
  friend class SocketConnection;

public:
  SocketAcceptor(DistributedATS::DATSApplication &, FIX::MessageStoreFactory &,
                 const FIX::SessionSettings &) throw(FIX::ConfigError);
  SocketAcceptor(DistributedATS::DATSApplication &, FIX::MessageStoreFactory &,
                 const FIX::SessionSettings &, FIX::LogFactory &/*,
                 std::ofstream */ ) throw(FIX::ConfigError);

  virtual ~SocketAcceptor();

  FIX::SocketServer *getSocketServer() { return m_pServer; };

  /*SocketConnection* getPendingLoginConnection(SessionID& sessionID)
  {
      return m_pendingLogonSocketConnection[sessionID];
  };*/

private:
  bool readSettings(const FIX::SessionSettings &);

  typedef std::set<FIX::SessionID> Sessions;
  typedef std::map<int, Sessions> PortToSessions;
  typedef std::map<int, SocketConnection *> SocketConnections;

  void onConfigure(const FIX::SessionSettings &) EXCEPT(ConfigError);
  void onInitialize(const FIX::SessionSettings &) EXCEPT(RuntimeError);

  void onStart() override;
  bool onPoll() override;
  void onStop() override;

  void onConnect(FIX::SocketServer &, int, int);
  void onWrite(FIX::SocketServer &, int);
  bool onData(FIX::SocketServer &, int);
  void onDisconnect(FIX::SocketServer &, int);
  void onError(FIX::SocketServer &);
  void onTimeout(FIX::SocketServer &);

  bool proceedWithLogon(const SessionID &);

  FIX::SocketServer *m_pServer;
  PortToSessions m_portToSessions;
  SocketConnections m_connections;

};
} // namespace DistributedATS

#endif // FIXGATEWAY_SOCKETACCEPTOR_H
