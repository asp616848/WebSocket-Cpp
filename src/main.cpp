#include <iostream>
#include "../include/WebSocket.h"
#include "../include/CLI.h"

int main() {
    WebSocket ws;
    CLI cli;

    if (ws.connect("wss://echo.websocket.events")) {
        std::cout << "Connected to server!" << std::endl;

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

            // Receive response from the server
            std::string response = ws.receiveMessage();
            if (response.empty()) {
                std::cerr << "No response from server. Exiting..." << std::endl;
                break;
            }

            // Display server response
            cli.displayMessage(response);
        }

        // Close the WebSocket connection
        ws.close();
        std::cout << "Connection closed." << std::endl;

    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }

    return 0;
}
