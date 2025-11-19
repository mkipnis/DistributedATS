// Copyright (C) Mike Kipnis - DistributedATS
#include "fix_application.h"
#include "ws_server.h"


void fix_application::toAdmin(FIX::Message& message, const FIX::SessionID& sessionID)
{
    const FIX::Header& header = message.getHeader();
    const FIX::MsgType& msgType = FIELD_GET_REF(header, MsgType);

    // Convert FIX message to JSON and add common fields
    auto json_obj = distributed_ats::fix_json::fix_to_json(message);
    json_obj["session_qualifier"] = sessionID.getSessionQualifier();
    json_obj["data_type"] = "FIX";
    json_obj["success"] = true;
    auto json_str = boost::json::serialize(json_obj);

    if (msgType == FIX::MsgType_Logon) {
        if (auto session = _ws_session.lock()) {
            auto ws_logon = session->get_pending_logon();

            std::cout << "\nSending Logon: " << sessionID
                      << " | Pending Logon: " << ws_logon << std::endl;

            // Set Username
            FIX::Username userName(sessionID.getSenderCompID());
            message.setField(userName);

            // Set Password from WebSocket logon
            FIX::Password password;
            ws_logon.getField(password);
            message.setField(password);

            // Print JSON log
            std::cout << "Sending Logon JSON: " << json_str << std::endl;
        }
    }
    else if (msgType == FIX::MsgType_Logout) {
        std::cout << "Logout JSON: " << json_str << std::endl;
    }

    // Optional: log via QuickFIX
    if (auto fix_session = FIX::Session::lookupSession(sessionID)) {
        if (auto log = fix_session->getLog()) {
            log->onEvent(json_str);
        }
    }
}


void fix_application::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionId)  throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon)
{
    // Convert FIX message to JSON
      auto json_obj = distributed_ats::fix_json::fix_to_json(message);
      json_obj["session_qualifier"] = sessionId.getSessionQualifier();
      json_obj["data_type"] = "FIX";
      json_obj["success"] = true;

      auto json_str = boost::json::serialize(json_obj);

      // Send JSON to WebSocket session
      if (auto session = _ws_session.lock()) {
          session->send_string(json_str);
      }

      // Log the JSON message using QuickFIX logger
      if (auto fix_session = FIX::Session::lookupSession(sessionId)) {
          if (auto log = fix_session->getLog()) {
              log->onEvent(json_str); // Log as a general event
              // If you prefer, you could also log as incoming FIX message:
              // log->onIncoming(message, sessionId);
          }
      }
}


void fix_application::fromApp(const FIX::Message& message, const FIX::SessionID& sessionId)  throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
{
    // Convert FIX message to JSON
       auto json_obj = distributed_ats::fix_json::fix_to_json(message);
       json_obj["session_qualifier"] = sessionId.getSessionQualifier();
       json_obj["data_type"] = "FIX";
       json_obj["success"] = true;

       auto json_str = boost::json::serialize(json_obj);

       // Send to your WebSocket session
       if (auto session = _ws_session.lock()) {
           session->send_string(json_str);
       }

       // Log the JSON message using QuickFIX logger
       if (auto fix_session = FIX::Session::lookupSession(sessionId)) {
           if (auto log = fix_session->getLog()) {
               log->onEvent(json_str);  // Log as an event
               // log->onIncoming(message, sessionId);
           }
       }
}
