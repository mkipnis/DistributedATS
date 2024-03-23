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

#include <LogonAdapter.hpp>
#include <MarketDataRequestAdapter.hpp>
#include <NewOrderSingleAdapter.hpp>
#include <OrderCancelRequestAdapter.hpp>
#include <OrderMassCancelRequestAdapter.hpp>
#include <OrderMassStatusRequestAdapter.hpp>
#include <OrderCancelReplaceRequestAdapter.hpp>
#include <SecurityListRequestAdapter.hpp>

#include "Application.hpp"
#include "SocketConnection.h"

#include <quickfix/fix44/ExecutionReport.h>

#include "SocketAcceptor.h"

#include "AuthServiceHelper.h"

#include <strstream>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

#include <LogonLogger.hpp>
#include <MarketDataRequestLogger.hpp>
#include <NewOrderSingleLogger.hpp>
#include <OrderCancelRequestLogger.hpp>
#include <OrderMassCancelRequestLogger.hpp>
#include <OrderMassStatusRequestLogger.hpp>
#include <SecurityListRequestLogger.hpp>
#include <OrderCancelReplaceRequestLogger.hpp>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include "DataWriterContainer.hpp"


using namespace DistributedATS;

Mutex DATSApplication::s_quickFIXSessionStateMutex;

//static auto logger = log4cxx::Logger::getRootLogger();

template <class ADAPTER, class DATA, class LOGGER>
void DATSApplication::publishToDDS(const FIX::Message &message,
                                   const distributed_ats_utils::data_writer_ptr& dataWriter,
                                   const std::string &sender,
                                   const std::string &target,
                                   const std::string &senderSubID) {
    DATA ddsMessage;
    
    try {
        ADAPTER::FIX2DDS(message, ddsMessage);
    } catch (FIX::FieldNotFound &field_not_found) {
        
        LOG4CXX_ERROR(logger, "Adapter message convertion : field not found : Type - [" << field_not_found.type << "] : Field - [" << field_not_found.field << "]" << " - " <<  message.toString());
        
        return;
    }
    
    ddsMessage.header().SenderCompID(sender);
    ddsMessage.header().TargetCompID(target);
    ddsMessage.header().SenderSubID(senderSubID);
    ddsMessage.header().TargetSubID(_dataService);
    
    std::stringstream ss;
    LOGGER::log(ss, ddsMessage);
    
    LOG4CXX_INFO(logger, "Publishing to DDS :" << ss.str());
    
    auto ret = dataWriter->write(&ddsMessage);
    
    if (!ret) {
        LOG4CXX_ERROR(logger, "Unable to publishing to DDS:" << ss.str());
    }
    
}

void DATSApplication::onCreate(const FIX::SessionID &sessionID) {
    LOG4CXX_INFO(logger, "Session Created :" <<  sessionID.toString());
}

void DATSApplication::onLogon(const FIX::SessionID &sessionID) {
    LOG4CXX_INFO(logger, "Session Logon :" <<  sessionID.toString());
}

void DATSApplication::onLogout(const FIX::SessionID &sessionID) {
    LOG4CXX_INFO(logger, "Session Logout :" << sessionID.toString());

  FIX::ClOrdID clOrdID;
  FIX::MassCancelRequestType massCancelRequestType(
      MassCancelRequestType_CANCEL_ALL_ORDERS);
  FIX::TransactTime transactType;

  FIX44::OrderMassCancelRequest message(clOrdID, massCancelRequestType,
                                        transactType);

  publishToDDS<OrderMassCancelRequestAdapter,
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest,
               OrderMassCancelRequestLogger>(
      message, _dataWriterContainer->_order_mass_cancel_request_dw,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onDisconnect(
    const FIX::SessionID &sessionID,
    DistributedATS::SocketConnection *pSocketConnection) {
    
    LOG4CXX_INFO(logger, "Session Disconnect :" << sessionID.toString());

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
            LOG4CXX_ERROR(logger, "unable to publish the message" <<  exp.what());
        }
    }

  } catch (FIX::FieldNotFound &field_not_found) {
      
      LOG4CXX_ERROR(logger, "Adapter message convertion : field "
                    "not found : Type - [" << field_not_found.type << "%] : Field - [" <<  field_not_found.field << "] - " <<
                    message.toString() << "]");

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
    DistributedATS_NewOrderSingle::NewOrderSingle, NewOrderSingleLogger>(
      message, _dataWriterContainer->_new_order_single_dw,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderCancelRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<OrderCancelRequestAdapter,
    DistributedATS_OrderCancelRequest::OrderCancelRequest,
               OrderCancelRequestLogger>(
      message, _dataWriterContainer->_order_cancel_request_dw,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderMassCancelRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<OrderMassCancelRequestAdapter,
    DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest,
               OrderMassCancelRequestLogger>(
      message, _dataWriterContainer->_order_mass_cancel_request_dw,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderCancelReplaceRequest &message, const FIX::SessionID &sessionID)
{
    publishToDDS<OrderCancelReplaceRequestAdapter,
    DistributedATS_OrderCancelReplaceRequest::OrderCancelReplaceRequest,
        OrderCancelReplaceRequestLogger>(
      message, _dataWriterContainer->_order_cancel_replace_request_dw,
      sessionID.getSenderCompID(), "MATCHING_ENGINE",
      sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::SecurityListRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<SecurityListRequestAdapter,
    DistributedATS_SecurityListRequest::SecurityListRequest,
               SecurityListRequestLogger>(
      message, _dataWriterContainer->_security_list_request_dw,
      sessionID.getSenderCompID(), "DATA_SERVICE", sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::MarketDataRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<MarketDataRequestAdapter,
    DistributedATS_MarketDataRequest::MarketDataRequest,
               MarketDataRequestLogger>(
      message, _dataWriterContainer->_market_data_request_dw,
      sessionID.getSenderCompID(), "DATA_SERVICE", sessionID.getTargetCompID());
}

void DATSApplication::onMessage(const FIX44::OrderMassStatusRequest &message,
                                const FIX::SessionID &sessionID) {
  publishToDDS<OrderMassStatusRequestAdapter,
    DistributedATS_OrderMassStatusRequest::OrderMassStatusRequest,
               OrderMassStatusRequestLogger>(
      message, _dataWriterContainer->_order_mass_status_request_dw,
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
    
    LOG4CXX_INFO(logger, "Connection Token :" << tokenStr);


  return tokenStr;
}

bool DATSApplication::insertPendingLogonSocketConnection(
    const Message &pendingLogon, SocketConnection *socketConnection) {
  FIX::TargetCompID clTargetCompID;
  pendingLogon.getHeader().getField(clTargetCompID);

  if (clTargetCompID.getValue().compare(this->_senderCompId) != 0) {
    LOG4CXX_DEBUG(logger, "Invalid Target CompID | Pending Logon :" << pendingLogon.toString());
      
    socketConnection->disconnect();
    return false;
  }

  std::string connectionToken = getConnectionToken(pendingLogon);
    
    LOG4CXX_DEBUG(logger, "Connection Token : " << connectionToken  << " | Pending Logon :" << pendingLogon.toString());

  _authService->insertPendingConnection(connectionToken, socketConnection);

  /*FIX::TargetCompID clTargetCompID;
  pendingLogon.getHeader().getField( clTargetCompID );*/
  FIX::SenderCompID clSenderCompID;
  pendingLogon.getHeader().getField(clSenderCompID);

  FIX::RawData tokenData(connectionToken);
  const_cast<Message &>(pendingLogon).setField(tokenData);

  publishToDDS<LogonAdapter, DistributedATS_Logon::Logon,
               LogonLogger>(
      pendingLogon, _dataWriterContainer->_logon_dw,
      clTargetCompID.getValue(), "AUTH", clSenderCompID.getValue());

  return true;
}
