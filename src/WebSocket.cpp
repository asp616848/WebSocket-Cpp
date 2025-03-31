#include "../include/WebSocket.h"
#define BOOST_BEAST_ALLOW_SSL_STREAM
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

// Custom teardown function for SSL WebSocket
void teardown(beast::role_type role, ssl::stream<tcp::socket>& ssl_socket, beast::error_code& ec) {
    ssl_socket.shutdown(ec);
}

// Define the implementation class
class WebSocket::Impl {
public:
    Impl() : ctx(ssl::context::tlsv12_client), ws(ioc, ctx) {}

    bool connect(const std::string& url) {
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

    bool sendMessage(const std::string& message) {
        try {
            ws.write(net::buffer(message));
            return true;
        } catch (const beast::system_error& se) {
            std::cerr << "Error: " << se.what() << "\n";
            return false;
        }
    }

    std::string receiveMessage() {
        try {
            beast::flat_buffer buffer;
            ws.read(buffer);
            return beast::buffers_to_string(buffer.data());
        } catch (const beast::system_error& se) {
            std::cerr << "Error receiving message: " << se.what() << "\n";
            return "";
        }
    }

    void close() {
        try {
            beast::error_code ec;
            ws.close(websocket::close_code::normal, ec);
            if (ec) {
                std::cerr << "Error closing WebSocket: " << ec.message() << "\n";
            }
        } catch (const beast::system_error& se) {
            std::cerr << "Error: " << se.what() << "\n";
        }
    }

private:
    net::io_context ioc;
    ssl::context ctx;
    websocket::stream<ssl::stream<tcp::socket>> ws;
};

// Proxy methods for WebSocket
WebSocket::WebSocket() : impl(std::make_unique<Impl>()) {}
WebSocket::~WebSocket() = default;

bool WebSocket::connect(const std::string& url) { return impl->connect(url); }
bool WebSocket::sendMessage(const std::string& message) { return impl->sendMessage(message); }
std::string WebSocket::receiveMessage() { return impl->receiveMessage(); }
void WebSocket::close() { impl->close(); }
