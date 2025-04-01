// WebSocketConnect.cpp
#include "../include/WebSocket.h"
#include <boost/beast/websocket/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = net::ssl;

// This function will be called from the Impl class
bool connectWebSocket(
    websocket::stream<ssl::stream<tcp::socket>>& ws,
    net::io_context& ioc,
    const std::string& url) {
    
    try {
        auto const pos = url.find("://");
        if (pos == std::string::npos) {
            std::cerr << "Invalid URL format\n";
            return false;
        }
        auto const host = url.substr(pos + 3);
        auto const port = "443";  // WebSocket Secure (WSS) port

        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(host, port);

        // Connect the underlying TCP layer
        net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str())) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            std::cerr << "Error setting SNI hostname: " << ec.message() << "\n";
            return false;
        }

        // Perform SSL Handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // Perform WebSocket Handshake
        ws.handshake(host, "/");

        std::cout << "Connected to " << host << "!\n";
        return true;
    } catch (const beast::system_error& se) {
        std::cerr << "Error: " << se.what() << "\n";
        return false;
    }
}
