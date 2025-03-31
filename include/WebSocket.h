// WebSocket.h
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <string>

class WebSocket {
public:
    bool connect(const std::string& url);
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void close();
};

#endif // WEBSOCKET_H

