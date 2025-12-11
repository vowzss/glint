#pragma once

#include <optional>
#include <string>

#include "glint/graphics/models/GeometryData.h"

namespace glint::engine::graphics {

    struct GeometryLoader {
      public:
        GeometryLoader() = default;
        ~GeometryLoader() = default;

        static std::optional<GeometryData> load(const std::string& filename);
    };

}