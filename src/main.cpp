#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include "../include/WebSocket.h"
#include "../include/CLI.h"

// Helper function to convert string to binary data
std::vector<uint8_t> stringToVector(const std::string& str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

// Helper function to convert binary data to string
std::string vectorToString(const std::vector<uint8_t>& data) {
    return std::string(data.begin(), data.end());
}

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

        std::cout << "Available commands:\n"
                  << "  text <message>  - Send text message\n"
                  << "  binary <message> - Send message as binary data\n"
                  << "  file <filename>  - Send binary file\n"
                  << "  exit            - Exit the program\n" << std::endl;

        while (true) {
            // Get user input
            std::string input = cli.getUserInput();
            
            // Check if the user wants to exit
            if (input == "exit") {
                std::cout << "Exiting communication..." << std::endl;
                break;
            }
            
            // Parse command
            std::string command, payload;
            size_t spacePos = input.find(' ');
            
            if (spacePos != std::string::npos) {
                command = input.substr(0, spacePos);
                payload = input.substr(spacePos + 1);
            } else {
                command = input;
                payload = "";
            }
            
            bool success = false;
            
            if (command == "text") {
                // Send as text message
                success = ws.sendMessage(payload);
                if (success) {
                    std::string response = ws.receiveMessage();
                    if (!response.empty()) {
                        cli.displayMessage("Text response: " + response);
                    } else {
                        std::cerr << "No response from server." << std::endl;
                    }
                }
            } 
            else if (command == "binary") {
                // Send as binary message
                std::vector<uint8_t> binaryData = stringToVector(payload);
                success = ws.sendBinaryMessage(binaryData);
                if (success) {
                    std::vector<uint8_t> response = ws.receiveBinaryMessage();
                    if (!response.empty()) {
                        cli.displayMessage("Binary response: " + vectorToString(response));
                        std::cout << "Received " << response.size() << " bytes" << std::endl;
                    } else {
                        std::cerr << "No binary response from server." << std::endl;
                    }
                }
            }
            else if (command == "file") {
                // Send file as binary
                std::ifstream file(payload, std::ios::binary);
                if (!file) {
                    std::cerr << "Failed to open file: " << payload << std::endl;
                    continue;
                }
                
                // Read file into vector
                file.seekg(0, std::ios::end);
                std::streamsize fileSize = file.tellg();
                file.seekg(0, std::ios::beg);
                
                std::vector<uint8_t> fileData(fileSize);
                if (file.read(reinterpret_cast<char*>(fileData.data()), fileSize)) {
                    std::cout << "Sending file of " << fileData.size() << " bytes..." << std::endl;
                    success = ws.sendBinaryMessage(fileData);
                    
                    if (success) {
                        std::vector<uint8_t> response = ws.receiveBinaryMessage();
                        if (!response.empty()) {
                            std::cout << "Received binary response of " << response.size() << " bytes" << std::endl;
                            // Only display as text if it seems to be text data
                            if (response.size() < 1000) { // Arbitrary limit to avoid huge output
                                cli.displayMessage("Response preview: " + vectorToString(response));
                            }
                        } else {
                            std::cerr << "No binary response from server." << std::endl;
                        }
                    }
                } else {
                    std::cerr << "Failed to read file: " << payload << std::endl;
                }
            }
            else {
                // Default to text message for backward compatibility
                success = ws.sendMessage(input);
                if (success) {
                    std::string response = ws.receiveMessage();
                    if (!response.empty()) {
                        cli.displayMessage(response);
                    } else {
                        std::cerr << "No response from server." << std::endl;
                    }
                }
            }
            
            if (!success) {
                std::cerr << "Failed to send message. Exiting..." << std::endl;
                break;
            }
        }
        
        ws.close();
        std::cout << "Connection closed." << std::endl;
    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }
    return 0;
}
