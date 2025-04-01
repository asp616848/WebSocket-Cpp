#include "../include/StringUtils.h"
#include <stdint.h>
#include <string>

std::vector<uint8_t> StringUtils::stringToVector(const std::string& str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

std::string StringUtils::vectorToString(const std::vector<uint8_t>& data) {
    return std::string(data.begin(), data.end());
}
