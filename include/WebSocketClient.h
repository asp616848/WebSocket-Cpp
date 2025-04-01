#pragma once

#include <string>
#include <vector>
#include "../include/WebSocket.h"
#include "../include/CLI.h"

class WebSocketClient {
public:
    WebSocketClient();
    bool connect(const std::string& url);
    void close();
    std::string receiveMessage();
    bool handleCommand(const std::string& input, CLI& cli);

private:
    WebSocket ws;
    bool sendTextMessage(const std::string& message, CLI& cli);
    bool sendBinaryMessage(const std::string& message, CLI& cli);
    bool sendFile(const std::string& filename, CLI& cli);
};
