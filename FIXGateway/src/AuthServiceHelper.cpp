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

#include "AuthServiceHelper.h"

#include <strstream>

#include <ace/Log_Msg.h>
#include <ace/Log_Msg_Callback.h>
#include <ace/Log_Record.h>
#include <ace/streams.h>

#include <quickfix/fix44/Logout.h>

AuthServiceHelper::AuthServiceHelper(
    std::shared_ptr<FIX::SessionSettings> settings,
    std::shared_ptr<FIX::SessionFactory> sessionFactory,
    const FIX::Dictionary *defaultDictionary, std::string senderCompID)
    : _settings(settings), _sessionFactory(sessionFactory),
      _defaultDictionary(defaultDictionary), _senderCompID(senderCompID) {}

//
// creates session id from provided message
//
const FIX::SessionID
AuthServiceHelper::SessionIDFromMessage(FIX::Message &message,
                                        const std::string &sessionQualifier) {
  FIX::BeginString beginString;
  FIX::SenderCompID clSenderCompID;
  FIX::TargetCompID clTargetCompID;

  message.getHeader().getField(beginString);
  message.getHeader().getField(clSenderCompID);
  message.getHeader().getField(clTargetCompID);

  FIX::SenderCompID senderCompID(clTargetCompID);
  FIX::TargetCompID targetCompID(clSenderCompID);
  return FIX::SessionID(beginString, senderCompID, targetCompID,
                        sessionQualifier);
}

//
// Messages from DataServer/Auth have AUTH in Source, we need to change it to
// Gateway Alias - i.e. EXECUTOR(Instance of the gateway) to proceed with login
//
FIX::Session *AuthServiceHelper::createSessionFromAuthMessage(
    const FIX::SessionID newFixSessionID, FIX::Message &message,
    const std::string &sessionQualifier) {

  FIX::TargetCompID currentTargetCompID;
  message.getHeader().getField(currentTargetCompID);
  FIX::TargetCompID clTargetCompId(newFixSessionID.getSenderCompID());

  message.getHeader().setField(clTargetCompId);
  message.getHeader().setField(currentTargetCompID);

  if (!_settings->has(newFixSessionID))
    _settings->set(newFixSessionID, *_defaultDictionary);

  FIX::Session *session = FIX::Session::lookupSession(newFixSessionID);

  try {
    if (session == NULL) {
      session = _sessionFactory->create(newFixSessionID, *_defaultDictionary);

      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("(%P|%t|%D) Creating QuickFIX Session from message "
                          "from auth service : %s\n"),
                 session->getSessionID().toString().c_str()));
    }
  } catch (FIX::ConfigError &cfgError) {
    std::cerr << "Config Exception in session create : " << cfgError.what()
              << std::endl;
  };

  return session;
};

void AuthServiceHelper::insertPendingConnection(
    const std::string &connectionToken,
    DistributedATS::SocketConnection *socketConnection) {
  FIX::Locker lock(_pendingSessionMutex);

  ACE_DEBUG(
      (LM_INFO,
       ACE_TEXT(
           "(%P|%t|%D) Inserting pending connection : %s : Socket : [%d]\n"),
       connectionToken.c_str(), socketConnection->getSocket()));

  std::cout << "Connection Token: " << connectionToken << std::endl;
  m_pendingLogonSocketConnection[connectionToken] = socketConnection;
  socketConnection->setPendingConnectionToken(connectionToken);
}

// returns sender compID
std::string AuthServiceHelper::getConnectionToken(const FIX::Message &message) {
  FIX::RawData rawData;
  message.getField(rawData);
  std::string connectionToken = rawData.getValue();

  return connectionToken;
}

void AuthServiceHelper::processDDSLogon(FIX::Message &message) {
  std::string connectionToken = getConnectionToken(message);

  FIX::BeginString beginString;
  FIX::TargetCompID clTargetCompID;
  FIX::SenderCompID clLogonSenderCompID;

  message.getHeader().getField(beginString);
  message.getHeader().getField(clTargetCompID);
  message.getHeader().getField(clLogonSenderCompID);

  FIX::SessionID newFixSessionID(beginString, _senderCompID, clTargetCompID,
                                 "");

  ACE_DEBUG(
      (LM_INFO,
       ACE_TEXT(
           "(%P|%t|%D) Received Session ID for Logon : %s : Session : [%s]\n"),
       _senderCompID.c_str(), newFixSessionID.toString().c_str()));

  FIX::Session *session = FIX::Session::lookupSession(newFixSessionID);

  if (session != NULL) {
    if (session->isLoggedOn() &&
        clLogonSenderCompID.getValue().compare(_senderCompID) == 0) {
      FIX44::Logout logoutMessage;

      FIX::Text logoutText("New session logged in with your credentials");
      logoutMessage.set(logoutText);

      FIX::Session::sendToTarget(logoutMessage, newFixSessionID);

      FIX::Locker lock(_pendingSessionMutex);
      m_sessionReloginMap[newFixSessionID] = message;

      return;
    } else {

      FIX44::Logout logoutMessage;

      FIX::Text logoutText(
          "New session logged in with your credentials from Another Gateway");
      logoutMessage.set(logoutText);

      FIX::Session::sendToTarget(logoutMessage, newFixSessionID);
    }
  } else if (clLogonSenderCompID.getValue().compare(_senderCompID) == 0) {
    session = AuthServiceHelper::createSessionFromAuthMessage(newFixSessionID,
                                                              message, "");

    if (session == NULL) {
      std::cerr << "Something is wrong unable to create session object!!!!!"
                << std::endl;
      return;
    }
  } else {
    return;
  }

  loginSession(session, connectionToken);
}

void AuthServiceHelper::loginSession(FIX::Session *session,
                                     std::string &connectionToken) {
  FIX::Locker lock(_pendingSessionMutex);

  ACE_DEBUG(
      (LM_INFO, ACE_TEXT("(%P|%t|%D) Login-in Session : [%s] - Token [%s] \n"),
       session->getSessionID().toString().c_str(), connectionToken.c_str()));

  DistributedATS::SocketConnection *responder =
      m_pendingLogonSocketConnection[connectionToken];

  if (responder != NULL) {
    responder->setSession(session);
    session->setResponder(responder);
    std::string pendingLogin = responder->getPendingLogin();

    m_pendingLogonSocketConnection.erase(connectionToken);

    try {
      session->next(pendingLogin, UtcTimeStamp());
    } catch (Exception &e) {
      ACE_DEBUG(
          (LM_ERROR,
           ACE_TEXT("(%P|%t|%D) Exception in Logon : [%s] - Exception [%s] \n"),
           session->getSessionID().toString().c_str(), e.what()));
    };

  } else {
    ACE_DEBUG(
        (LM_INFO,
         ACE_TEXT("(%P|%t|%D) Responder not found : [%s] - Token [%s] \n"),
         session->getSessionID().toString().c_str(), connectionToken.c_str()));
  }
}

void AuthServiceHelper::processDDSLogout(std::string &connectionToken,
                                         FIX::Message &message) {
  FIX::Locker lock(_pendingSessionMutex);

  DistributedATS::SocketConnection *responder =
      m_pendingLogonSocketConnection[connectionToken];

  if (responder == NULL)
    return;

  std::string pendingLoginStr = responder->getPendingLogin();

  FIX::Message pendingLogin(pendingLoginStr);

  FIX::BeginString beginString;
  FIX::Username clUsername;

  FIX::SenderCompID clSenderCompID;
  FIX::TargetCompID clTargetCompID;

  pendingLogin.getHeader().getField(beginString);
  pendingLogin.getHeader().getField(clTargetCompID);
  pendingLogin.getHeader().getField(clSenderCompID);

  FIX::MsgSeqNum msgSeqNum(1);
  FIX::SendingTime sendingTime;
  FIX::TargetCompID targetCompID(clSenderCompID.getValue());
  FIX::SenderCompID senderCompID(clTargetCompID.getValue());

  message.getHeader().setField(msgSeqNum);
  message.getHeader().setField(targetCompID);
  message.getHeader().setField(senderCompID);
  message.getHeader().setField(sendingTime);

  ACE_DEBUG((LM_INFO,
             ACE_TEXT("(%P|%t|%D) Logout : Original SessionID Token [%s] : "
                      "Logout Message ID: %s\n"),
             connectionToken.c_str(), message.toString().c_str()));

  responder->send(message.toString());
  responder->disconnect();
}

void AuthServiceHelper::processDisconnect(
    const FIX::SessionID &sessionID,
    DistributedATS::SocketConnection *pSocketConnection) {
  FIX::Locker lock(_pendingSessionMutex);

  ACE_DEBUG(
      (LM_INFO,
       ACE_TEXT("(%P|%t|%D) Disconnecting Pending Connection Token: %s \n"),
       pSocketConnection->getPendingConnectionToken().c_str()));

  // lets check if its a pending connection
  auto responder_iterator = m_pendingLogonSocketConnection.find(
      pSocketConnection->getPendingConnectionToken());
  if (responder_iterator != m_pendingLogonSocketConnection.end()) {
    // it a pending connection disconnect
    m_pendingLogonSocketConnection.erase(responder_iterator);
    return;
  }

  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Disconneting : [%s]\n"),
             sessionID.toString().c_str()));

  SessionReLoginMap::iterator pendingLoginIn =
      m_sessionReloginMap.find(sessionID);
  if (pendingLoginIn != m_sessionReloginMap.end()) {
    ACE_DEBUG(
        (LM_INFO,
         ACE_TEXT("(%P|%t|%D) There is a session to relogin : [%s] : [%s]\n"),
         sessionID.toString().c_str(),
         pendingLoginIn->second.toString().c_str()));

    FIX::Session *session = FIX::Session::lookupSession(sessionID);
    session->logon();

    std::string connectionToken = getConnectionToken(pendingLoginIn->second);

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Connection Token : [%s]\n"),
               connectionToken.c_str()));

    loginSession(session, connectionToken);

    m_sessionReloginMap.erase(pendingLoginIn);

  } else {
    FIX::Session *session = FIX::Session::lookupSession(sessionID);
    session->setResponder(NULL);
    session->logout("Disconnected");
    _sessionFactory->destroy(session);
  }
}
