#include <iostream>
#include <string>
#include "../include/WebSocketClient.h"
#include "../include/CLI.h"

int main() {
    WebSocketClient wsClient;
    CLI cli;

    if (wsClient.connect("wss://echo.websocket.events")) {
        std::cout << "Connected to server!" << std::endl;
        
        // Display welcome message if any
        std::string welcome = wsClient.receiveMessage();
        if (!welcome.empty()) {
            cli.displayMessage(welcome);
        }

        std::cout << "Available commands:\n"
                  << "  text <message>  - Send text message\n"
                  << "  binary <message> - Send message as binary data\n"
                  << "  file <filename>  - Send binary file\n"
                  << "  exit            - Exit the program\n" << std::endl;

        while (true) {
            std::string input = cli.getUserInput();
            
            if (input == "exit") {
                std::cout << "Exiting communication..." << std::endl;
                break;
            }
            
            if (!wsClient.handleCommand(input, cli)) {
                std::cerr << "Failed to send message. Exiting..." << std::endl;
                break;
            }
        }
        
        wsClient.close();
        std::cout << "Connection closed." << std::endl;
    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }
    return 0;
}
