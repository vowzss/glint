#include "glint/utils/file_utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace glint;

namespace fs = std::filesystem;

std::vector<char> utils::read_file(const std::string& filename) {
    // todo: fix degen path find
    fs::path exePath = fs::current_path().parent_path().parent_path().parent_path().parent_path();
    fs::path rssPath = exePath / "resources";
    fs::path fullPath = rssPath / filename;

    std::cout << fullPath << "\n";

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