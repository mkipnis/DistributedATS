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

#include "SocketAcceptor.h"
#include <quickfix/Exceptions.h>
#include <quickfix/Session.h>
#include <quickfix/Settings.h>
#include <quickfix/Utility.h>

#include "SocketConnection.h"

using namespace FIX;

namespace DistributedATS {
SocketAcceptor::SocketAcceptor(
    DistributedATS::DATSApplication &application, MessageStoreFactory &factory,
    const SessionSettings &settings) throw(ConfigError)
    : Acceptor(application, factory, settings), m_pServer(0) {}

SocketAcceptor::SocketAcceptor(
    DistributedATS::DATSApplication &application, MessageStoreFactory &factory,
    const SessionSettings &settings, FIX::LogFactory &logFactory/*,
    std::ofstream *dds_input_stream_log_file*/) throw(ConfigError)
    : Acceptor(application, factory, settings, logFactory), m_pServer(0) {}

SocketAcceptor::~SocketAcceptor() {
  SocketConnections::iterator iter;
  for (iter = m_connections.begin(); iter != m_connections.end(); ++iter)
    delete iter->second;
}

void SocketAcceptor::onConfigure(const SessionSettings &s) throw(ConfigError) {
  std::set<SessionID> sessions = s.getSessions();
  std::set<SessionID>::iterator i;
  for (i = sessions.begin(); i != sessions.end(); ++i) {
    const Dictionary &settings = s.get(*i);
    settings.getInt(SOCKET_ACCEPT_PORT);
    if (settings.has(SOCKET_REUSE_ADDRESS))
      settings.getBool(SOCKET_REUSE_ADDRESS);
    if (settings.has(SOCKET_NODELAY))
      settings.getBool(SOCKET_NODELAY);
  }
}

void SocketAcceptor::onInitialize(const SessionSettings &s) throw(
    RuntimeError) {
  short port = 0;

  try {
    m_pServer = new FIX::SocketServer(1);

    std::set<SessionID> sessions = s.getSessions();
    std::set<SessionID>::iterator i = sessions.begin();
    for (; i != sessions.end(); ++i) {
      const Dictionary &settings = s.get(*i);
      port = (short)settings.getInt(SOCKET_ACCEPT_PORT);

      const bool reuseAddress = settings.has(SOCKET_REUSE_ADDRESS)
                                    ? settings.getBool(SOCKET_REUSE_ADDRESS)
                                    : true;

      const bool noDelay = settings.has(SOCKET_NODELAY)
                               ? settings.getBool(SOCKET_NODELAY)
                               : false;

      const int sendBufSize = settings.has(SOCKET_SEND_BUFFER_SIZE)
                                  ? settings.getInt(SOCKET_SEND_BUFFER_SIZE)
                                  : 0;

      const int rcvBufSize = settings.has(SOCKET_RECEIVE_BUFFER_SIZE)
                                 ? settings.getInt(SOCKET_RECEIVE_BUFFER_SIZE)
                                 : 0;

      m_portToSessions[port].insert(*i);
      m_pServer->add(port, reuseAddress, noDelay, sendBufSize, rcvBufSize);
    }
  } catch (SocketException &e) {
    throw RuntimeError("Unable to create, bind, or listen to port " +
                       IntConvertor::convert((unsigned short)port) + " (" +
                       e.what() + ")");
  }
}

void SocketAcceptor::onStart() {
    
  while (!isStopped() && m_pServer && m_pServer->block(*this)) {
    // ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%T|%D) Socket Acceptor\n")));
  }

  if (!m_pServer)
    return;

  time_t start = 0;
  time_t now = 0;

  ::time(&start);
  while (isLoggedOn()) {
    m_pServer->block(*this);
    if (::time(&now) - 5 >= start)
      break;
  }

  m_pServer->close();
  delete m_pServer;
  m_pServer = 0;
}

bool SocketAcceptor::onPoll() {
  if (!m_pServer)
    return false;

  time_t start = 0;
  time_t now = 0;

  if (isStopped()) {
    if (start == 0)
      ::time(&start);
    if (!isLoggedOn()) {
      start = 0;
      return false;
    }
    if (::time(&now) - 5 >= start) {
      start = 0;
      return false;
    }
  }

  std::cout << "Polling" << std::endl;
  m_pServer->block(*this, true);
  return true;
}

void SocketAcceptor::onStop() {}

void SocketAcceptor::onConnect(FIX::SocketServer &server, int a, int s) {
  if (!socket_isValid(s))
    return;
  SocketConnections::iterator i = m_connections.find(s);
  if (i != m_connections.end())
    return;
  int port = server.socketToPort(a);
  Sessions sessions = m_portToSessions[port];
  m_connections[s] = new SocketConnection(s, sessions, &server.getMonitor());

  std::stringstream stream;
  stream << "Accepted connection from " << socket_peername(s) << " on port "
         << port;

  if (getLog())
    getLog()->onEvent(stream.str());
}

void SocketAcceptor::onWrite(FIX::SocketServer &server, int s) {
  SocketConnections::iterator i = m_connections.find(s);
  if (i == m_connections.end())
    return;
  SocketConnection *pSocketConnection = i->second;
  if (pSocketConnection->processQueue())
    pSocketConnection->unsignal();
}

bool SocketAcceptor::onData(FIX::SocketServer &server, int s) {
  SocketConnections::iterator i = m_connections.find(s);
  if (i == m_connections.end())
    return false;
  SocketConnection *pSocketConnection = i->second;
  return pSocketConnection->read(*this, server);
}

void SocketAcceptor::onDisconnect(FIX::SocketServer &, int s) {
  SocketConnections::iterator i = m_connections.find(s);
  if (i == m_connections.end())
    return;
  SocketConnection *pSocketConnection = i->second;

  FIX::Session *pSession = pSocketConnection->getSession();
  if (pSession) {
    pSession->disconnect();
    DistributedATS::DATSApplication &application =
        dynamic_cast<DistributedATS::DATSApplication &>(getApplication());
    application.onDisconnect(pSession->getSessionID(), pSocketConnection);
  }

    // TODO: this will leak - review pSocketConnection lifecycle
  //delete pSocketConnection;
  m_connections.erase(s);
}

void SocketAcceptor::onError(FIX::SocketServer &) {}

void SocketAcceptor::onTimeout(FIX::SocketServer &) {
  /*  SocketConnections::iterator i;
    for ( i = m_connections.begin(); i != m_connections.end(); ++i )
      i->second->onTimeout();
  */
}

/* void SocketAcceptor::insertPendingLogonSocketConnection(const SessionID&
 sessionID, SocketConnection* socketConnection )
 {
     m_pendingLogonSocketConnection[sessionID] = socketConnection;
 }*/

} // namespace DistributedATS
