#include "glint/utils/file_utils.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace glint;

std::vector<char> utils::read_file(const std::string& filename) {

    std::string rssPath = std::string(PROJECT_ROOT) + "/resources/";
    std::string fullPath = rssPath + filename;

    std::ifstream file(fullPath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Failed to read file: " + filename);
    }

    return buffer;
}