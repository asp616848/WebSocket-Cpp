#include <iostream>
#include "../include/WebSocket.h"
#include "../include/CLI.h"
int main() {
    WebSocket ws;
    CLI cli;

    if (ws.connect("wss://echo.websocket.events")) {
        std::cout << "Connected to server!" << std::endl;
        
        // There might be a welcome message
        std::string welcome = ws.receiveMessage();
        if (!welcome.empty()) {
            cli.displayMessage(welcome);
        }

        while (true) {
            // Get user input
            std::string message = cli.getUserInput();
        
            // Check if the user wants to exit
            if (message == "exit") {
                std::cout << "Exiting communication..." << std::endl;
                break;
            }
        
            // Send message to the server
            if (!ws.sendMessage(message)) {
                std::cerr << "Failed to send message. Exiting..." << std::endl;
                break;
            }
        
            // Wait for and display the response from the server
            std::string response = ws.receiveMessage();
            if (response.empty()) {
                std::cerr << "No response from server. Exiting..." << std::endl;
                break;
            }
            cli.displayMessage(response);
        }
        
        ws.close();
        std::cout << "Connection closed." << std::endl;
    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }
    return 0;
}
