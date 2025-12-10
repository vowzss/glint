#pragma once

#include <string>
#include <vector>

namespace glint::engine::utils::files {

    std::string getResourcePath(const std::string& filename);
    std::string getModelPath(const std::string& filename);

    std::vector<char> read(const std::string& filename);

}