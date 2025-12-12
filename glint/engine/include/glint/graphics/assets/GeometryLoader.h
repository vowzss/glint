#pragma once

#include <optional>
#include <string>

#include "Geometry.h"

namespace glint::engine::graphics {

    struct GeometryLoader {
      public:
        GeometryLoader() noexcept = default;
        ~GeometryLoader() noexcept = default;

        static std::optional<Geometry> load(const std::string& filename);
    };

}