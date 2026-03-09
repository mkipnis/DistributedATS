// Copyright (C) Mike Kipnis - DistributedATS
#pragma once

#include <quickfix/SessionSettings.h>
#include <quickfix/SessionFactory.h>
#include <quickfix/NullStore.h>
#include <quickfix/ThreadedSocketInitiator.h>

class Session;

class fix_application : public FIX::Application {

public:
    
    fix_application(const std::weak_ptr<Session>& ws_session) : _ws_session(ws_session) {}
    
    void onCreate(const FIX::SessionID& id) override {
        std::cout << "Created: " << id << std::endl;
    }
    void onLogon(const FIX::SessionID& id) override {
        std::cout << "Logon: " << id << std::endl;
    }
    void onLogout(const FIX::SessionID& id) override {
        std::cout << "Logout: " << id << std::endl;
    }
    void toAdmin(FIX::Message& message, const FIX::SessionID& sessionID) override;
    
    void fromAdmin(const FIX::Message &,
                 const FIX::SessionID &) noexcept(false) override;
    void fromApp(const FIX::Message &message,
          const FIX::SessionID &sessionID) noexcept(false) override;


    void toApp(FIX::Message&, const FIX::SessionID&) noexcept(false) override {};

private:
    std::weak_ptr<Session> _ws_session;
};

