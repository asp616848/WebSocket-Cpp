#pragma once

#include <string>
#include <stdint.h>
#include <vector>

class StringUtils {
public:
    static std::vector<uint8_t> stringToVector(const std::string& str);
    static std::string vectorToString(const std::vector<uint8_t>& data);
};
