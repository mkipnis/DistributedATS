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

#ifndef FIXGATEWAY_SOCKETCONNECTION_H
#define FIXGATEWAY_SOCKETCONNECTION_H

#include <quickfix/Mutex.h>
#include <quickfix/Parser.h>
#include <quickfix/Responder.h>
#include <quickfix/SessionID.h>
#include <quickfix/SocketInitiator.h>
#include <quickfix/SocketMonitor.h>
#include <quickfix/Utility.h>
#include <set>

using namespace FIX;

namespace DistributedATS {
class SocketServer;
class SocketConnector;
class Session;
class SocketInitiator;
class SocketAcceptor;

/// Encapsulates a socket file descriptor (single-threaded).
class SocketConnection : public Responder {
public:
  typedef std::set<SessionID> Sessions;

  SocketConnection(int s, Sessions sessions, SocketMonitor *pMonitor);
  /* SocketConnection( FIX::SocketInitiator&, const SessionID&, int,
   * SocketMonitor* );*/
  virtual ~SocketConnection();

  int getSocket() const { return m_socket; }
  FIX::Session *getSession() const { return m_pSession; }

  bool read(FIX::SocketConnector &s);
  bool read(SocketAcceptor &, FIX::SocketServer &);
  bool processQueue();

  void signal() {
    Locker l(m_mutex);
    if (m_sendQueue.size() == 1)
      m_pMonitor->signal(m_socket);
  }

  void setSession(FIX::Session *pSession) { m_pSession = pSession; }

  void unsignal() {
    Locker l(m_mutex);
    if (m_sendQueue.size() == 0)
      m_pMonitor->unsignal(m_socket);
  }

  void onTimeout();

  std::string getPendingLogin() { return m_pendingLogin; };

  bool isValidSession();
  bool send(const std::string &);
  void disconnect();

  void setPendingConnectionToken(const std::string &pendingConnectionToken) {
    m_pendingConnectionToken = pendingConnectionToken;
  }

  std::string getPendingConnectionToken() { return m_pendingConnectionToken; }

private:
  typedef std::deque<std::string, ALLOCATOR<std::string>> Queue;

  void readFromSocket() throw(SocketRecvFailed);
  bool readMessage(std::string &msg);
  void readMessages(SocketMonitor &s);

  int m_socket;
  char m_buffer[BUFSIZ];

  Parser m_parser;
  Queue m_sendQueue;
  unsigned m_sendLength;
  Sessions m_sessions;
  FIX::Session *m_pSession;
  SocketMonitor *m_pMonitor;
  Mutex m_mutex;
  fd_set m_fds;

  std::string m_pendingLogin;
  std::string m_pendingConnectionToken; // to deal with disconnects, while login
                                        // in flight
};
} // namespace DistributedATS

#endif // FIX_SOCKETCONNECTION_H
