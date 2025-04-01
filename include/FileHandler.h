#pragma once

#include <string>
#include <stdint.h>
#include <vector>

class FileHandler {
public:
    static std::vector<uint8_t> readFile(const std::string& filename);
};
