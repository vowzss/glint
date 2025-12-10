#pragma once

#include <optional>
#include <string>

namespace glint::engine::graphics {

    struct GeometryData;

    struct GeometryLoader {
      public:
        GeometryLoader() = default;
        ~GeometryLoader() = default;

        static std::optional<GeometryData> load(const std::string& filename);
    };

}