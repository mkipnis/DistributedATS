// Copyright (C) Mike Kipnis - DistributedATS
#include "fix_application.h"
#include "ws_server.h"


void fix_application::toAdmin(FIX::Message& message, const FIX::SessionID& sessionID)
{
    const FIX::Header& header = message.getHeader();

    const FIX::MsgType& msgType = FIELD_GET_REF( header, MsgType );

    if ( msgType == FIX::MsgType_Logon )
    {
        if (auto session = _ws_session.lock()) {
            auto ws_logon = session->get_pending_logon();
            
            std::cout << std::endl << "Sending Logon : " << sessionID << "Pending Logon : " << ws_logon << std::endl;
            
            FIX::Username userName(sessionID.getSenderCompID());
            message.setField(userName);
            
            FIX::Password password;
            ws_logon.getField(password);
            
            message.setField(password);
            
            std::cout << "Sending Logon : " << message.toString() << std::endl;
        }
      } else if (  msgType == FIX::MsgType_Logout )
      {
          std::cout << "Logout : " <<  message << std::endl;
      }
}

void fix_application::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionId)  throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon)
{
    if (auto session = _ws_session.lock()) {
     
        auto json_obj = distributed_ats::fix_json::fix_to_json(message);
        
        json_obj["session_qualifier"] = sessionId.getSessionQualifier();
        json_obj["data_type"] = "FIX";
        json_obj["success"] = true;
        
        auto json_str = boost::json::serialize(json_obj);

        session->send_string(json_str);
    }
}


void fix_application::fromApp(const FIX::Message& message, const FIX::SessionID& sessionId)  throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
{
    if (auto session = _ws_session.lock()) {
     
        auto json_obj = distributed_ats::fix_json::fix_to_json(message);
        
        json_obj["session_qualifier"] = sessionId.getSessionQualifier();
        json_obj["data_type"] = "FIX";
        json_obj["success"] = true;
        
        auto json_str = boost::json::serialize(json_obj);

        session->send_string(json_str);
    }
}
