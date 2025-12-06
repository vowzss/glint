#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace glint::engine::utils::files {

    std::filesystem::path getResourcePath(const std::string& filename);
    std::filesystem::path getModelPath(const std::string& filename);

    std::vector<char> read(const std::string& filename);

}