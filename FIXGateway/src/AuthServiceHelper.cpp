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

#include "AuthServiceHelper.h"

#include <strstream>
#include <quickfix/fix44/Logout.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

static auto logger = log4cxx::Logger::getRootLogger();


ActiveUserMap AuthServiceHelper::m_activeUserMap;

AuthServiceHelper::AuthServiceHelper(
    std::shared_ptr<FIX::SessionSettings> settings,
    std::shared_ptr<FIX::SessionFactory> sessionFactory,
    std::shared_ptr<FIX::Dictionary> defaultDictionary, std::string senderCompID)
    : _settings(settings), _sessionFactory(sessionFactory),
      _defaultDictionary(defaultDictionary), _senderCompID(senderCompID) {}

//
// creates session id from provided message
//

bool
AuthServiceHelper::ActiveSessionIDFromMessage(const FIX::Message &message,
                                      FIX::SessionID& sessionID) {
  FIX::SenderCompID clTargetCompID;
  
  message.getHeader().getField(clTargetCompID);
  auto activeSessionID = m_activeUserMap.find(clTargetCompID.getValue());

  if ( activeSessionID != m_activeUserMap.end() )
  {
      sessionID = activeSessionID->second;
      return true;
  }
    
  return false;
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

        LOG4CXX_INFO(logger, "Creating QuickFIX Session from message "
                      "from auth service :" << session->getSessionID() );
     
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
    
    
    LOG4CXX_INFO(logger, "Inserting pending connection : [" << connectionToken << "] Socket :[" << socketConnection->getSocket()<<"]");

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

    auto connectionToken = getConnectionToken(message);
    
    auto beginString = message.getHeader().getField(FIX::FIELD::BeginString);
    //auto senderCompID = message.getHeader().getField(FIX::FIELD::SenderCompID);
    auto targetCompID = message.getHeader().getField(FIX::FIELD::TargetCompID);
    auto targetSubID = message.getHeader().getField(FIX::FIELD::TargetSubID);
    auto clSessionQualifier = message.getField(FIX::FIELD::RawData);

    LOG4CXX_INFO(logger, "Received Session ID for Logon : [" << _senderCompID.c_str() << "] Session :[" << clSessionQualifier<<"]");
    
    FIX::Session *session = NULL;

    auto activeSessionID = m_activeUserMap.find(targetSubID);
  
    if ( activeSessionID != m_activeUserMap.end() )
      session =  FIX::Session::lookupSession(activeSessionID->second);
  


  if (session != NULL) {
    // Reconnection to the same gateway
    if (session->isLoggedOn() &&
        targetCompID.compare(_senderCompID) == 0)
    {
      FIX44::Logout logoutMessage;

      FIX::Text logoutText("New session logged in with your credentials");
      logoutMessage.set(logoutText);

      std::cout << "Sending logout: " << activeSessionID->second << std::endl;
        
      FIX::Session::sendToTarget(logoutMessage, activeSessionID->second);
      m_activeUserMap.erase(activeSessionID);

    } else { // Connection from another gateway

      FIX44::Logout logoutMessage;

      FIX::Text logoutText(
          "New session logged in with your credentials from Another Gateway");
      logoutMessage.set(logoutText);

      FIX::Session::sendToTarget(logoutMessage, activeSessionID->second);
      m_activeUserMap.erase(activeSessionID);

      return;
    }
  }
    
  if (targetCompID.compare(_senderCompID) == 0)
    {
        FIX::SessionID newFixSessionID(beginString, _senderCompID, targetSubID,
                                       clSessionQualifier);
       session = AuthServiceHelper::createSessionFromAuthMessage(newFixSessionID,
                                                              message, "");
        
    if (session)
    {
        m_activeUserMap[targetSubID] = newFixSessionID;
    } else {
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
    
    LOG4CXX_INFO(logger, "Login-in Session : [" << session->getSessionID() << "] Token :[" << connectionToken<<"]");


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
        
        LOG4CXX_ERROR(logger, "Exception in Logon : [" << session->getSessionID() << "] Exception :[" << e.what()<<"]");

    };

  } else {
      
      LOG4CXX_INFO(logger, "Responder not found : [" << session->getSessionID() << "] Token :[" << connectionToken <<"]");

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
    
    LOG4CXX_INFO(logger, "Logout : Original SessionID Token [%s] : [" << connectionToken << "] Logout Message :[" << message.toString() <<"]");

  responder->send(message.toString());
  responder->disconnect();
}

void AuthServiceHelper::processDisconnect(
    const FIX::SessionID &sessionID,
    DistributedATS::SocketConnection *pSocketConnection) {
  FIX::Locker lock(_pendingSessionMutex);
    
    LOG4CXX_INFO(logger, "Disconnecting Pending Connection Token: [" << pSocketConnection->getPendingConnectionToken() );


  // lets check if its a pending connection
  auto responder_iterator = m_pendingLogonSocketConnection.find(
      pSocketConnection->getPendingConnectionToken());
  if (responder_iterator != m_pendingLogonSocketConnection.end()) {
    // it a pending connection disconnect
    m_pendingLogonSocketConnection.erase(responder_iterator);
    return;
  }

    LOG4CXX_INFO(logger, "Disconnecting : [" <<  sessionID.toString() );
    

    FIX::Session *session = FIX::Session::lookupSession(sessionID);
    session->setResponder(NULL);
    session->logout("Disconnected");
    _sessionFactory->destroy(session);
    
}
