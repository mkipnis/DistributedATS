// Copyright (C) Mike Kipnis - DistributedATS

#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <fix_json_converter.h>

#include <quickfix/SessionSettings.h>
#include <quickfix/SessionFactory.h>
#include <quickfix/NullStore.h>
#include <quickfix/ThreadedSocketInitiator.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <thread>

namespace asio  = boost::asio;
namespace beast = boost::beast;
namespace http  = beast::http;
namespace ws    = beast::websocket;
using tcp       = asio::ip::tcp;


class fix_application;

// A single WebSocket session (handles one client)
class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket, const std::stringstream& session_settings_stream)
        : ws_(std::move(socket))
    {
        std::stringstream ss(session_settings_stream.str());
        settings_ = std::make_shared<FIX::SessionSettings>(ss);
    }

    void start() {
        // Accept the websocket handshake
        ws_.async_accept(
            beast::bind_front_handler(&Session::on_accept, shared_from_this()));
    }
    
    ~Session() {
                
        boost::shared_lock<boost::shared_mutex> lock(rw_mutex_);
        try {
            if (socket_initator_) {
                socket_initator_->stop();
                socket_initator_.reset();
            }
        } catch (std::exception const& e) {
            std::cerr << "Exception in Session destructor: " << e.what() << std::endl;
        }
    }

private:
    ws::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    
    std::string fix_session_qualifier_;
    std::string ws_session_qualifier_;
    
    std::shared_ptr<FIX::SessionSettings> settings_;
    std::shared_ptr<FIX::NullStoreFactory> storeFactory_;
    std::shared_ptr<FIX::ScreenLogFactory> logFactory_;
    
    
    std::shared_ptr<FIX::Application> application_;
    std::shared_ptr<FIX::ThreadedSocketInitiator> socket_initator_;

    FIX::Message pending_logon_;
    
    boost::shared_mutex rw_mutex_;

    void on_accept(beast::error_code ec) {
        if (ec) {
            std::cerr << "accept error: " << ec.message() << "\n";
            return;
        }

        auto& tcp_stream = ws_.next_layer();
        auto remote_endpoint = tcp_stream.remote_endpoint();
        auto local_endpoint  = tcp_stream.local_endpoint();

        ws_session_qualifier_ =
            local_endpoint.address().to_string() + ":" + std::to_string(local_endpoint.port()) +
            " <-> " +
            remote_endpoint.address().to_string() + ":" + std::to_string(remote_endpoint.port());
        
        boost::uuids::uuid token = boost::uuids::random_generator()();
        fix_session_qualifier_ = boost::uuids::to_string(token);
        
        do_read();
    }

    void do_read() {
        buffer_.consume(buffer_.size()); // ensure buffer empty
        ws_.async_read(
            buffer_,
            beast::bind_front_handler(&Session::on_read, shared_from_this()));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred);

    void on_write(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec) {
            std::cerr << "write error: " << ec.message() << "\n";
            return;
        }
        // Clear the buffer and read another message
        buffer_.consume(buffer_.size());
        do_read();
    }
    
    void on_close() {
        
        boost::shared_lock<boost::shared_mutex> lock(rw_mutex_);

        std::cout << "[WS] Connection closed: " << ws_session_qualifier_ << std::endl;

        // Gracefully stop the FIX initiator if running
        if (socket_initator_) {
            try {
                socket_initator_->stop();
                socket_initator_.reset();
            } catch (std::exception const& e) {
                std::cerr << "Error stopping FIX initiator: " << e.what() << std::endl;
            }
        }
    }
    
public:
    
    const FIX::Message& get_pending_logon()
    {
        return pending_logon_;
    }
    
    void send_string(const std::string& message) {
        
            boost::unique_lock<boost::shared_mutex> lock(rw_mutex_);
        
            asio::dispatch(ws_.get_executor(), [self = shared_from_this(), message]() {
                try {
                    self->ws_.text(true);
                    self->ws_.write(boost::asio::buffer(message)); // synchronous write
                } catch (const std::exception& e) {
                    std::cerr << "sync write error: " << e.what() << std::endl;
                }
            });
        
            /*
            auto self = shared_from_this(); // keep session alive during async write
            ws_.text(true); // send as text
            ws_.async_write(
                boost::asio::buffer(message),
                [self](beast::error_code ec, std::size_t bytes_transferred){
                    boost::ignore_unused(bytes_transferred);
                    if (ec) {
                        std::cerr << "write error: " << ec.message() << "\n";
                    }
                }
            );*/
        }
};


// Accepts incoming connections and launches sessions
class Listener : public std::enable_shared_from_this<Listener> {
public:
    Listener(asio::io_context& ioc, tcp::endpoint endpoint, std::shared_ptr<FIX::SessionSettings>& fix_session_settings)
        : ioc_(ioc), acceptor_(ioc)
    {
        session_settings_stream_ << *fix_session_settings;
        
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        if (ec) throw beast::system_error(ec);

        acceptor_.set_option(asio::socket_base::reuse_address(true), ec);
        if (ec) throw beast::system_error(ec);

        acceptor_.bind(endpoint, ec);
        if (ec) throw beast::system_error(ec);

        acceptor_.listen(asio::socket_base::max_listen_connections, ec);
        if (ec) throw beast::system_error(ec);
    }

    void run() {
        do_accept();
    }

private:
    asio::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::stringstream session_settings_stream_;

    void do_accept() {
        acceptor_.async_accept(
            asio::make_strand(ioc_),
            beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
        if (ec) {
            std::cerr << "accept failed: " << ec.message() << "\n";
        } else {
            // Create and run session
            std::make_shared<Session>(std::move(socket), session_settings_stream_)->start();
        }
        do_accept();
    }
};

