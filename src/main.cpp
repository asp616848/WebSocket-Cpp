// main.cpp
#include <iostream>
#include "../include/WebSocket.h"

int main() {
    WebSocket ws;
    if (ws.connect("wss://echo.websocket.events")) {
        std::cout << "Connected to server!" << std::endl;
        ws.sendMessage("Hello, World!");
        std::cout << "Received: " << ws.receiveMessage() << std::endl;
        ws.close();
    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }
    return 0;
}
