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

#ifndef __AUTH_SERVICE_HELPER_H__
#define __AUTH_SERVICE_HELPER_H__

#include <quickfix/FileStore.h>
#include <quickfix/Log.h>
#include <quickfix/SessionFactory.h>
#include <quickfix/SessionSettings.h>

#include "SocketConnection.h"

class AuthServiceHelper {
  std::shared_ptr<FIX::SessionSettings> _settings;
  std::shared_ptr<FIX::SessionFactory> _sessionFactory;
  const FIX::Dictionary *_defaultDictionary;

  typedef std::map<const std::string, DistributedATS::SocketConnection *>
      PendingLogonSocketConnection;
  typedef std::map<const FIX::SessionID, FIX::Message> SessionReLoginMap;

  PendingLogonSocketConnection m_pendingLogonSocketConnection;
  SessionReLoginMap m_sessionReloginMap;

  FIX::Mutex _pendingSessionMutex;

public:
  AuthServiceHelper(std::shared_ptr<FIX::SessionSettings> settings,
                    std::shared_ptr<FIX::SessionFactory> sessionFactory,
                    const FIX::Dictionary *defaultDictionary,
                    std::string senderCompID);

  static const FIX::SessionID
  SessionIDFromMessage(FIX::Message &message,
                       const std::string &sessionQualifier = "");

private:
  FIX::Session *
  createSessionFromAuthMessage(const FIX::SessionID newFixSessionID,
                               FIX::Message &message,
                               const std::string &sessionQualifier);

  // returns sender compID
  std::string getConnectionToken(const FIX::Message &);

  std::string _senderCompID;

public:
  void
  insertPendingConnection(const std::string &,
                          DistributedATS::SocketConnection *socketConnection);
  void processDDSLogon(FIX::Message &message);
  void processDDSLogout(std::string &connectionToken, FIX::Message &message);
  void processDisconnect(const FIX::SessionID &sessionID,
                         DistributedATS::SocketConnection *pSocketConnection);

  void loginSession(FIX::Session *session, std::string &connectionToken);
};

#endif
