#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <string>
#include <memory> // For smart pointers

class WebSocket {
public:
    WebSocket();  // Constructor
    ~WebSocket(); // Destructor

    bool connect(const std::string& url);
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void close();

private:
    class Impl;                     // Forward declaration of implementation class
    std::unique_ptr<Impl> impl;     // Pointer to implementation
};

#endif // WEBSOCKET_H
