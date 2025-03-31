#include "../include/WebSocket.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio; // from <boost/asio.hpp>
using tcp = net::ip::tcp;

class WebSocket {
public:
    bool connect(const std::string& url) {
        try {
            // Resolve the server address
            auto const pos = url.find("://");
            auto const host = url.substr(pos + 3);
            
            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve(host, "80");

            // Connect to the server
            net::connect(ws.next_layer(), results.begin(), results.end());

            // Perform the WebSocket handshake
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
    websocket::stream<tcp::socket> ws{ioc};
};
