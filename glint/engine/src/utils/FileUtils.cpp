#include "glint/utils/FileUtils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace glint::engine::utils {
    std::vector<char> readFile(const std::string& filename) {
        fs::path rssPath = fs::absolute("resources");
        fs::path fullPath = rssPath / filename;

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
}