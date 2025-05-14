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
#include "SocketConnection.h"

using namespace FIX;

namespace DistributedATS {

SocketConnection::SocketConnection(int s, Sessions sessions,
                                   SocketMonitor *pMonitor)
    : m_socket(s), m_sendLength(0), m_sessions(sessions), m_pSession(0),
      m_pMonitor(pMonitor) {
  FD_ZERO(&m_fds);
  FD_SET(m_socket, &m_fds);
}

/* SocketConnection::SocketConnection( FIX::SocketInitiator& i,
                                 const SessionID& sessionID, int s,
                                 SocketMonitor* pMonitor )
: m_socket( s ), m_sendLength( 0 ),
m_pSession( i.getSession( sessionID, *this ) ),
m_pMonitor( pMonitor )
{
FD_ZERO( &m_fds );
FD_SET( m_socket, &m_fds );
m_sessions.insert( sessionID );
}*/

SocketConnection::~SocketConnection() {

  if (m_pSession)
  {
    //m_pSession->setResponder( 0 );
    FIX::Session::unregisterSession(m_pSession->getSessionID());
  }
}

bool SocketConnection::send(const std::string &msg) {
  Locker l(m_mutex);

  m_sendQueue.push_back(msg);
  processQueue();
  signal();
  return true;
}

bool SocketConnection::processQueue() {
  Locker l(m_mutex);

  if (!m_sendQueue.size())
    return true;

  struct timeval timeout = {0, 0};
  fd_set writeset = m_fds;
  if (select(1 + m_socket, 0, &writeset, 0, &timeout) <= 0)
    return false;

  const std::string &msg = m_sendQueue.front();

  ssize_t result = socket_send(m_socket, msg.c_str() + m_sendLength,
                               msg.length() - m_sendLength);

  if (result > 0)
    m_sendLength += result;

  if (m_sendLength == msg.length()) {
    m_sendLength = 0;
    m_sendQueue.pop_front();
  }

  return !m_sendQueue.size();
}

void SocketConnection::disconnect() {
  if (m_pMonitor)
    m_pMonitor->drop(m_socket);
}

bool SocketConnection::read(FIX::SocketConnector &s) {
  if (!m_pSession)
    return false;

  try {
    readFromSocket();
    readMessages(s.getMonitor());
  } catch (SocketRecvFailed &e) {
    m_pSession->getLog()->onEvent(e.what());
    return false;
  }
  return true;
}

bool SocketConnection::read(SocketAcceptor &a, FIX::SocketServer &s) {
  std::string msg;
  try {
    if (!m_pSession) {
      struct timeval timeout = {1, 0};
      fd_set readset = m_fds;

      while (!readMessage(msg)) {
        int result = select(1 + m_socket, &readset, 0, 0, &timeout);
        if (result > 0)
          readFromSocket();
        else if (result == 0)
          return false;
        else if (result < 0)
          return false;
      }

      m_pSession = FIX::Session::lookupSession(msg, true);

        
        
      //
      // Check if there is/was already an active session, don't disconnect the current session but
      // send new session credential for authentication to DataService.
      // This is done to handle reconnects with the same credentials.
      // If Autheticaion succeeds, existing session will be disconnection and new session activated/re-activated
      //
      if (!isValidSession() ||
          (m_pSession != NULL && m_pSession->isLoggedOn())) {

        Message message;
        if (!message.setStringHeader(msg))
          return false;

        try {
          const FIX::Header &header = message.getHeader();

          const MsgType &msgType = FIELD_GET_REF(header, MsgType);
  
          if (msgType == MsgType_Logon) {
            m_pSession = 0;

            DistributedATS::DATSApplication &app =
                dynamic_cast<DistributedATS::DATSApplication &>(
                    a.getApplication());

            m_pendingLogin = msg;
            return app.insertPendingLogonSocketConnection(msg, this);
          }

        } catch (FieldNotFound &) {
          return false;
        }

        if (a.getLog()) {
          a.getLog()->onEvent("Session not found for incoming message: " + msg);
          a.getLog()->onIncoming(msg);
        }
      }
      if (m_pSession)
        m_pSession = a.getSession(msg, *this);
      if (m_pSession)
        m_pSession->next(msg, UtcTimeStamp());
      if (!m_pSession) {
        s.getMonitor().drop(m_socket);
        return false;
      }

      FIX::Session::registerSession(m_pSession->getSessionID());
      return true;
    } else {
      readFromSocket();
      readMessages(s.getMonitor());
      return true;
    }
  } catch (SocketRecvFailed &e) {
    if (m_pSession)
      m_pSession->getLog()->onEvent(e.what());
    s.getMonitor().drop(m_socket);
  } catch (InvalidMessage &) {
    s.getMonitor().drop(m_socket);
  }
  return false;
}

bool SocketConnection::isValidSession() {
  if (m_pSession == 0)
    return false;
  SessionID sessionID = m_pSession->getSessionID();
  if (FIX::Session::isSessionRegistered(sessionID))
    return false;
  return !(m_sessions.find(sessionID) == m_sessions.end());
}

void SocketConnection::readFromSocket() throw(SocketRecvFailed) {
  ssize_t size = socket_recv(m_socket, m_buffer, sizeof(m_buffer));
  if (size <= 0)
    throw SocketRecvFailed(size);
  m_parser.addToStream(m_buffer, size);
}

bool SocketConnection::readMessage(std::string &msg) {
  try {
    return m_parser.readFixMessage(msg);
  } catch (MessageParseError &) {
  }
  return true;
}

void SocketConnection::readMessages(SocketMonitor &s) {
  if (!m_pSession)
    return;

  std::string msg;
  while (readMessage(msg)) {
    try {
      m_pSession->next(msg, UtcTimeStamp());
    } catch (InvalidMessage &) {
      if (!m_pSession->isLoggedOn())
        s.drop(m_socket);
    }
  }
}

void SocketConnection::onTimeout() {
  if (m_pSession)
    m_pSession->next();
}
} // namespace DistributedATS
