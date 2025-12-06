#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "glint/utils/FileUtils.h"

namespace glint::engine::utils::files {

    namespace fs = std::filesystem;

    fs::path getResourcePath(const std::string& filename) {
        fs::path path = fs::current_path() / "resources";
        fs::path fullPath = path / filename;

        return fullPath;
    }

    fs::path getModelPath(const std::string& filename) {
        fs::path path = fs::current_path() / "resources" / "models";
        fs::path fullPath = path / filename;

        return fullPath;
    }

    std::vector<char> read(const std::string& filename) {
        fs::path path = getResourcePath(filename);
        std::ifstream file(path, std::ios::ate | std::ios::binary);

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