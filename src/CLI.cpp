// CLI.cpp
#include "CLI.h"
#include <iostream>

std::string CLI::getUserInput() {
    std::string input;
    std::cout << "Enter message: ";
    std::getline(std::cin, input);
    return input;
}

void CLI::displayMessage(const std::string& message) {
    std::cout << "Server: " << message << std::endl;
}
