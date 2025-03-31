#include "../include/WebSocket.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Define the implementation class
class WebSocket::Impl {
public:
    Impl() : ws(ioc) {}

    bool connect(const std::string& url) {
        try {
            auto const pos = url.find("://");
            auto const host = url.substr(pos + 3);

            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve(host, "80");

            net::connect(ws.next_layer(), results.begin(), results.end());
            ws.handshake(host, "/");
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
            std::cerr << "Error: " << se.what() << "\n";
            return "";
        }
    }

    void close() {
        try {
            ws.close(websocket::close_code::normal);
        } catch (const beast::system_error& se) {
            std::cerr << "Error: " << se.what() << "\n";
        }
    }

private:
    net::io_context ioc;
    websocket::stream<tcp::socket> ws;
};

// Proxy methods for WebSocket
WebSocket::WebSocket() : impl(std::make_unique<Impl>()) {}
WebSocket::~WebSocket() = default;

bool WebSocket::connect(const std::string& url) { return impl->connect(url); }
bool WebSocket::sendMessage(const std::string& message) { return impl->sendMessage(message); }
std::string WebSocket::receiveMessage() { return impl->receiveMessage(); }
void WebSocket::close() { impl->close(); }
