#include "../include/FileHandler.h"
#include <fstream>
#include <iostream>
#include <stdint.h>

std::vector<uint8_t> FileHandler::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return std::vector<uint8_t>();
    }
    
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> fileData(fileSize);
    if (file.read(reinterpret_cast<char*>(fileData.data()), fileSize)) {
        return fileData;
    } else {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return std::vector<uint8_t>();
    }
}
