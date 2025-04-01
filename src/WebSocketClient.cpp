#include "../include/WebSocketClient.h"
#include "../include/FileHandler.h"
#include "../include/StringUtils.h"
#include <iostream>

WebSocketClient::WebSocketClient() {}

bool WebSocketClient::connect(const std::string& url) {
    return ws.connect(url);
}

void WebSocketClient::close() {
    ws.close();
}

std::string WebSocketClient::receiveMessage() {
    return ws.receiveMessage();
}

bool WebSocketClient::handleCommand(const std::string& input, CLI& cli) {
    std::string command, payload;
    size_t spacePos = input.find(' ');
    
    if (spacePos != std::string::npos) {
        command = input.substr(0, spacePos);
        payload = input.substr(spacePos + 1);
    } else {
        command = input;
        payload = "";
    }
    
    if (command == "text") {
        return sendTextMessage(payload, cli);
    } else if (command == "binary") {
        return sendBinaryMessage(payload, cli);
    } else if (command == "file") {
        return sendFile(payload, cli);
    } else {
        return sendTextMessage(input, cli);
    }
}

bool WebSocketClient::sendTextMessage(const std::string& message, CLI& cli) {
    if (ws.sendMessage(message)) {
        std::string response = ws.receiveMessage();
        if (!response.empty()) {
            cli.displayMessage("Text response: " + response);
        } else {
            std::cerr << "No response from server." << std::endl;
        }
        return true;
    }
    return false;
}

bool WebSocketClient::sendBinaryMessage(const std::string& message, CLI& cli) {
    std::vector<uint8_t> binaryData = StringUtils::stringToVector(message);
    if (ws.sendBinaryMessage(binaryData)) {
        std::vector<uint8_t> response = ws.receiveBinaryMessage();
        if (!response.empty()) {
            cli.displayMessage("Binary response: " + StringUtils::vectorToString(response));
            std::cout << "Received " << response.size() << " bytes" << std::endl;
        } else {
            std::cerr << "No binary response from server." << std::endl;
        }
        return true;
    }
    return false;
}

bool WebSocketClient::sendFile(const std::string& filename, CLI& cli) {
    std::vector<uint8_t> fileData = FileHandler::readFile(filename);
    if (fileData.empty()) {
        return false;
    }
    
    std::cout << "Sending file of " << fileData.size() << " bytes..." << std::endl;
    if (ws.sendBinaryMessage(fileData)) {
        std::vector<uint8_t> response = ws.receiveBinaryMessage();
        if (!response.empty()) {
            std::cout << "Received binary response of " << response.size() << " bytes" << std::endl;
            if (response.size() < 1000) {
                cli.displayMessage("Response preview: " + StringUtils::vectorToString(response));
            }
        } else {
            std::cerr << "No binary response from server." << std::endl;
        }
        return true;
    }
    return false;
}
