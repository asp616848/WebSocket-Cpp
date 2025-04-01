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
void teardown( ssl::stream<tcp::socket>& ssl_socket, beast::error_code& ec) {
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
            ws.text(true);  // Set to text mode
            ws.write(net::buffer(message));
            return true;
        } catch (const beast::system_error& se) {
            std::cerr << "Error: " << se.what() << "\n";
            return false;
        }
    }

    bool sendBinaryMessage(const std::vector<uint8_t>& data) {
        try {
            ws.binary(true);  // Set to binary mode
            ws.write(net::buffer(data));
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

    std::vector<uint8_t> receiveBinaryMessage() {
        try {
            beast::flat_buffer buffer;
            ws.read(buffer);
            
            // Convert buffer to vector<uint8_t>
            auto data = buffer.data();
            auto size = buffer.size();
            const unsigned char* bytes = static_cast<const unsigned char*>(data.data());
            return std::vector<uint8_t>(bytes, bytes + size);
        } catch (const beast::system_error& se) {
            std::cerr << "Error receiving binary message: " << se.what() << "\n";
            return std::vector<uint8_t>();
        }
    }

    void setBinaryMode(bool binary) {
        binary_mode = binary;
        ws.binary(binary);
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
    bool binary_mode;
};

// Proxy methods for WebSocket
WebSocket::WebSocket() : impl(std::make_unique<Impl>()) {}
WebSocket::~WebSocket() = default;

bool WebSocket::connect(const std::string& url) { return impl->connect(url); }
bool WebSocket::sendMessage(const std::string& message) { return impl->sendMessage(message); }
bool WebSocket::sendBinaryMessage(const std::vector<uint8_t>& data) { return impl->sendBinaryMessage(data); }
std::string WebSocket::receiveMessage() { return impl->receiveMessage(); }
std::vector<uint8_t> WebSocket::receiveBinaryMessage() { return impl->receiveBinaryMessage(); }
void WebSocket::setBinaryMode(bool binary) { impl->setBinaryMode(binary); }
void WebSocket::close() { impl->close(); }