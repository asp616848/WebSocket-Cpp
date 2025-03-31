// CLI.h
#ifndef CLI_H
#define CLI_H

#include <string>

class CLI {
public:
    std::string getUserInput();
    void displayMessage(const std::string& message);
};

#endif // CLI_H
