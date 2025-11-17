// Copyright (C) Mike Kipnis - DistributedATS

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <fix_json_converter.h>

#include "fix_application.h"
#include "ws_server.h"

using boost::asio::ip::tcp;
namespace asio = boost::asio;
namespace po = boost::program_options;


namespace asio  = boost::asio;
namespace beast = boost::beast;
namespace http  = beast::http;
namespace ws    = beast::websocket;
using tcp       = asio::ip::tcp;


int main(int argc, char** argv) {
    try {
        // Default values
        auto const address = asio::ip::make_address("0.0.0.0");
        unsigned short port = 9002;
        std::string fix_client_config;

        po::options_description desc("Allowed options");
        desc.add_options()
                   ("help,h", "produce help message")
                   ("port,p", po::value<unsigned short>(), "set WebSocket listening port")
                   ("fix_client_config,f", po::value<std::string>(), "set FIX client configuration file");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("port"))
            port = vm["port"].as<unsigned short>();

        if (vm.count("fix_client_config"))
            fix_client_config = vm["fix_client_config"].as<std::string>();

        std::cout << "Using port: " << port << "\n";
        if (!fix_client_config.empty())
            std::cout << "Using FIX client config file: " << fix_client_config << "\n";
        
        
        auto fix_session_setttings = std::make_shared<FIX::SessionSettings>(fix_client_config);
        

        asio::io_context ioc{1}; // number of ws threads
        std::make_shared<Listener>(ioc, tcp::endpoint{address, port}, fix_session_setttings)->run();

        std::cout << "WebSocket echo server listening on port " << port << "\n";
        ioc.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
