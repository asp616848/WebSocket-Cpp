// CLI.cpp
#include "../include/CLI.h"
#include <iostream>

// Implementation of getUserInput()
std::string CLI::getUserInput() {
    std::string input;
    std::cout << "Enter message: ";
    std::getline(std::cin, input);
    return input;
}

// Implementation of displayMessage()
void CLI::displayMessage(const std::string& message) {
    std::cout << "Server: " << message << std::endl;
}
