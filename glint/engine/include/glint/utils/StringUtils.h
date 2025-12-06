#pragma once
#include <sstream>
#include <string>
#include <vector>

namespace glint::engine::utils::string {

    template <typename T>
    std::string join(const std::vector<T>& vec, const std::string& sep = ", ") {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            oss << vec[i];
            if (i + 1 < vec.size()) oss << sep;
        }
        oss << "]";
        return oss.str();
    }

}
