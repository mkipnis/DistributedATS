// Copyright (C) Mike Kipnis - DistributedATS

#include "ws_server.h"
#include "fix_application.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <quickfix/Message.h>
#include <quickfix/Session.h>
#include <quickfix/SessionID.h>


void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    
    boost::shared_lock<boost::shared_mutex> lock(rw_mutex_);
    
    boost::ignore_unused(bytes_transferred);
    if (ec == ws::error::closed) return;
    if (ec) {
        std::cerr << "read error: " << ec.message() << "\n";
        return;
    }
    
    auto message = beast::buffers_to_string(buffer_.data());
    auto fix_message = distributed_ats::fix_json::json_to_fix(message);
    auto ws_session_id = fix_message.getSessionID();
    
    FIX::SessionID qualified_id(
        ws_session_id.getBeginString(),
        ws_session_id.getSenderCompID(),
        ws_session_id.getTargetCompID(),
        fix_session_qualifier_
    );
    
    boost::json::object replay;
    if ( !FIX::Session::doesSessionExist(qualified_id) )
    {
        settings_->set(qualified_id, FIX::Dictionary());
        
        pending_logon_ = fix_message;
        
        storeFactory_ = std::make_shared<FIX::NullStoreFactory>();
        logFactory_ = std::make_shared<FIX::ScreenLogFactory>( *settings_ );
        
        auto json_obj = distributed_ats::fix_json::fix_to_json(fix_message);
        auto json_str = boost::json::serialize(json_obj);
        std::cout << "FIX Message : " << json_str << std::endl;
        
        application_ = std::make_shared<fix_application>(shared_from_this());
        socket_initator_ = std::make_shared<FIX::ThreadedSocketInitiator>( *application_, *storeFactory_, *settings_, *logFactory_ );
        
        socket_initator_->start();
        replay["token"] = fix_session_qualifier_;
        replay["success"] = true;
    } else {

        //std::cout << "About to send: " << fix_message << std::endl;
        
        auto json_obj = distributed_ats::fix_json::fix_to_json(fix_message);
        auto json_str = boost::json::serialize(json_obj);
        std::cout << "FIX Message : " << json_str << std::endl;
        
        replay["token"] = fix_session_qualifier_;
        
        bool success = FIX::Session::sendToTarget(fix_message, qualified_id);
        if (!success)
            std::cerr << "Failed to send FIX message to " << qualified_id << std::endl;
        
        replay["success"] = success;

    }
    
    replay["data_type"] = "WS";
    std::string replay_str = boost::json::serialize(replay);

    // Send over WebSocket
    ws_.text(true); // ensure it's sent as text
    ws_.async_write(
        boost::asio::buffer(replay_str),
        beast::bind_front_handler(&Session::on_write, shared_from_this())
    );
}
