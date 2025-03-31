#include <boost/asio/ssl.hpp>

class WebSocket {
public:
    WebSocket();
    ~WebSocket();
    bool connect(const std::string& url);
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void close();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};
