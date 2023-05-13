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

#include <LogonAdapter.hpp>
#include <LogonTypeSupportImpl.h>
#include <MarketDataRequestAdapter.hpp>
#include <MarketDataRequestTypeSupportImpl.h>
#include <NewOrderSingleAdapter.hpp>
#include <NewOrderSingleTypeSupportImpl.h>
#include <OrderCancelRequestAdapter.hpp>
#include <OrderCancelRequestTypeSupportImpl.h>
#include <OrderMassCancelRequestAdapter.hpp>
#include <OrderMassCancelRequestTypeSupportImpl.h>
#include <OrderMassStatusRequestAdapter.hpp>
#include <OrderMassStatusRequestTypeSupportImpl.h>
#include <OrderCancelReplaceRequestAdapter.hpp>
#include <OrderCancelReplaceRequestTypeSupportImpl.h>
#include <SecurityListRequestAdapter.hpp>
#include <SecurityListRequestTypeSupportImpl.h>

#include <ace/Message_Queue_T.h>

#include "Application.hpp"
#include "SocketConnection.h"

#include <quickfix/fix44/ExecutionReport.h>

#include <dds/DdsDcpsPublicationC.h>

#include "SocketAcceptor.h"

#include "AuthServiceHelper.h"

#include <strstream>

#include <LogonLogger.hpp>
#include <MarketDataRequestLogger.hpp>
#include <NewOrderSingleLogger.hpp>
#include <OrderCancelRequestLogger.hpp>
#include <OrderMassCancelRequestLogger.hpp>
#include <OrderMassStatusRequestLogger.hpp>
#include <SecurityListRequestLogger.hpp>
#include <OrderCancelReplaceRequestLogger.hpp>

#include "DataWriterContainer.hpp"

using namespace DistributedATS;

Mutex DATSApplication::s_quickFIXSessionStateMutex;

template <class ADAPTER, class DATA_WRITER, class DATA, class LOGGER>
void DATSApplication::publishToDDS(const FIX::Message &message,
                                   DATA_WRITER &dataWriter,
                                   const std::string &sender,
                                   const std::string &target,
                                   const std::string &senderSubID) {
  DATA ddsMessage;

  try {
    ADAPTER::FIX2DDS(message, ddsMessage);
  } catch (FIX::FieldNotFound &field_not_found) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("(%P|%t|%D) ERROR: Adapter message convertion : field "
                        "not found : Type - [%s] : Field - [%d] - %s.\n"),
               field_not_found.type.c_str(), field_not_found.field,
               message.toString().c_str()));
    return;
  }

  ddsMessage.m_Header.SenderCompID = CORBA::string_dup(sender.c_str());
  ddsMessage.m_Header.TargetCompID = CORBA::string_dup(target.c_str());
  ddsMessage.m_Header.SenderSubID = CORBA::string_dup(senderSubID.c_str());
  ddsMessage.m_Header.TargetSubID = CORBA::string_dup(_dataService.c_str());

  std::stringstream ss;
  LOGGER::log(ss, ddsMessage);
  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) Publishing to DDS : %s\n"),
             ss.str().c_str()));

  int ret = dataWriter->write(ddsMessage, NULL);

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("(%P|%t) ERROR: Data Writer write returned %d.\n"),
               ret));
  }
}

void DATSApplication::onCreate(const FIX::SessionID &sessionID) {
  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) Session Created : %s\n"),
             sessionID.toString().c_str()));
}

void DATSApplication::onLogon(const FIX::SessionID &sessionID) {
  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) Session Logon : %s\n"),
             sessionID.toString().c_str()));
}

void DATSApplication::onLogout(const FIX::SessionID &sessionID) {
  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) Session Logout : %s\n"),
             sessionID.toString().c_str()));

  FIX::ClOrdID clOrdID;
  FIX::MassCancelRequestType massCancelRequestType(
      MassCancelRequestType_CANCEL_ALL_ORDERS);
  FIX::TransactTime transactType;

  FIX44::OrderMassCancelRequest message(clOrdID, massCancelRequestType,
                                        transactType);

  publishToDDS<OrderMassCancelRequestAdapter,
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestDataWriter_ptr,
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest,
               OrderMassCancelRequestLogger>(
      message, _dataWriterContainer->_orderMassCancelRequestDataWriter,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onDisconnect(
    const FIX::SessionID &sessionID,
    DistributedATS::SocketConnection *pSocketConnection) {
  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) Session Disconnect : %s\n"),
             sessionID.toString().c_str()));
  FIX::Locker lock(s_quickFIXSessionStateMutex);
  _authService->processDisconnect(sessionID, pSocketConnection);
}

void DATSApplication::toAdmin(FIX::Message &message,
                              const FIX::SessionID &sessionID) {}
void DATSApplication::toApp(
    FIX::Message &message,
    const FIX::SessionID &sessionID) throw(FIX::DoNotSend) {}

void DATSApplication::fromAdmin(
    const FIX::Message &message,
    const FIX::SessionID &sessionID) throw(FIX::FieldNotFound,
                                           FIX::IncorrectDataFormat,
                                           FIX::IncorrectTagValue,
                                           FIX::RejectLogon) {}

void DATSApplication::fromApp(
    const FIX::Message &message,
    const FIX::SessionID &sessionID) throw(FIX::FieldNotFound,
                                           FIX::IncorrectDataFormat,
                                           FIX::IncorrectTagValue,
                                           FIX::UnsupportedMessageType) {
  crack(message, sessionID);
}

void DATSApplication::publishToClient(FIX::Message &message) {
    
    FIX::Locker lock( s_quickFIXSessionStateMutex );
    
    try {
    
    FIX::SessionID activeSessionID;

    if (AuthServiceHelper::ActiveSessionIDFromMessage(message, activeSessionID))
    {
        try {
            FIX::Session::sendToTarget(message, activeSessionID);
        } catch ( std::exception& exp )
        {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("(%P|%t|%D) ERROR: unable to publish the message [%s] \n"),
                       exp.what()));
        }
    }

  } catch (FIX::FieldNotFound &field_not_found) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("(%P|%t|%D) ERROR: Adapter message convertion : field "
                        "not found : Type - [%s] : Field - [%d] - %s.\n"),
               field_not_found.type.c_str(), field_not_found.field,
               message.toString().c_str()));
    return;
  }
}

void DATSApplication::broadcastToClients(FIX::Message &message) {

    FIX::Locker lock(s_quickFIXSessionStateMutex);
    
    for (auto sessionID : FIX::Session::getSessions()) {
    FIX::Session::sendToTarget(message, sessionID);
  }
}

void DATSApplication::onMessage(const FIX44::NewOrderSingle &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<NewOrderSingleAdapter,
    DistributedATS_NewOrderSingle::NewOrderSingleDataWriter_ptr,
    DistributedATS_NewOrderSingle::NewOrderSingle, NewOrderSingleLogger>(
      message, _dataWriterContainer->_newOrderSingleDataWriter,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderCancelRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<OrderCancelRequestAdapter,
    DistributedATS_OrderCancelRequest::OrderCancelRequestDataWriter_ptr,
    DistributedATS_OrderCancelRequest::OrderCancelRequest,
               OrderCancelRequestLogger>(
      message, _dataWriterContainer->_orderCancelRequestDataWriter,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderMassCancelRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<OrderMassCancelRequestAdapter,
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequestDataWriter_ptr,
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest,
               OrderMassCancelRequestLogger>(
      message, _dataWriterContainer->_orderMassCancelRequestDataWriter,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderCancelReplaceRequest &message, const FIX::SessionID &sessionID)
{
    publishToDDS<OrderCancelReplaceRequestAdapter,
    DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequestDataWriter_ptr,
    DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest,
        OrderCancelReplaceRequestLogger>(
      message, _dataWriterContainer->_orderCancelReplaceRequestDataWriter,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::SecurityListRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<SecurityListRequestAdapter,
    DistributedATS_SecurityListRequest::SecurityListRequestDataWriter_ptr,
    DistributedATS_SecurityListRequest::SecurityListRequest,
               SecurityListRequestLogger>(
      message, _dataWriterContainer->_securityListRequestDataWriter,
      sessionID.getSenderCompID(), "DATA_SERVICE", sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::MarketDataRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<MarketDataRequestAdapter,
    DistributedATS_MarketDataRequest::MarketDataRequestDataWriter_ptr,
    DistributedATS_MarketDataRequest::MarketDataRequest,
               MarketDataRequestLogger>(
      message, _dataWriterContainer->_marketDataRequestDataWriter,
      sessionID.getSenderCompID(), "DATA_SERVICE", sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderMassStatusRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<OrderMassStatusRequestAdapter,
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequestDataWriter_ptr,
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest,
               OrderMassStatusRequestLogger>(
      message, _dataWriterContainer->_orderMassStatusRequestDataWriter,
      sessionID.getSenderCompID(), "DATA_SERVICE", sessionID.getTargetCompID());
}

void DATSApplication::processDDSLogon(FIX::Message &message) {
    FIX::Locker lock(s_quickFIXSessionStateMutex);
  _authService->processDDSLogon(message);
}

void DATSApplication::processDDSLogout(std::string &connectionToken,
                                       FIX::Message &message) {
    FIX::Locker lock(s_quickFIXSessionStateMutex);
  _authService->processDDSLogout(connectionToken, message);
}

std::string
DATSApplication::getConnectionToken(const FIX::Message &pendingLogon) {
  FIX::UtcTimeStamp utcTimStamp;

  FIX::SenderCompID clSenderCompID;
  FIX::TargetCompID clTargetCompID;

  pendingLogon.getHeader().getField(clSenderCompID);
  pendingLogon.getHeader().getField(clTargetCompID);

  std::stringstream tokenStream;
  tokenStream << clSenderCompID.getValue() << ":" << clTargetCompID.getValue()
              << ":" << utcTimStamp.getTimeT() << ":" << _connection_id++;

  std::string tokenStr = tokenStream.str().c_str();

  ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t|%D) Connection Token: %s \n"),
             tokenStr.c_str()));

  return tokenStr;
}

bool DATSApplication::insertPendingLogonSocketConnection(
    const Message &pendingLogon, SocketConnection *socketConnection) {
  FIX::TargetCompID clTargetCompID;
  pendingLogon.getHeader().getField(clTargetCompID);

  if (clTargetCompID.getValue().compare(this->_senderCompId) != 0) {
    ACE_DEBUG(
        (LM_INFO,
         ACE_TEXT("(%P|%t|%D) Invalid Target CompID | Pending Logon : %s \n"),
         pendingLogon.toString().c_str()));
    socketConnection->disconnect();
    return false;
  }

  std::string connectionToken = getConnectionToken(pendingLogon);

  ACE_DEBUG(
      (LM_INFO,
       ACE_TEXT("(%P|%t|%D) Connection Token : %s | Pending Logon : %s \n"),
       connectionToken.c_str(), pendingLogon.toString().c_str()));

  _authService->insertPendingConnection(connectionToken, socketConnection);

  /*FIX::TargetCompID clTargetCompID;
  pendingLogon.getHeader().getField( clTargetCompID );*/
  FIX::SenderCompID clSenderCompID;
  pendingLogon.getHeader().getField(clSenderCompID);

  FIX::RawData tokenData(connectionToken);
  const_cast<Message &>(pendingLogon).setField(tokenData);

  publishToDDS<LogonAdapter, DistributedATS_Logon::LogonDataWriter_ptr, DistributedATS_Logon::Logon,
               LogonLogger>(
      pendingLogon, _dataWriterContainer->_logonDataWriter,
      clTargetCompID.getValue(), "AUTH", clSenderCompID.getValue());

  return true;
}
