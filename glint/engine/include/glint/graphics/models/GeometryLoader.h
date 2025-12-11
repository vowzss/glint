#pragma once

#include <optional>
#include <string>

#include "GeometryData.h"

namespace glint::engine::graphics {

    struct GeometryLoader {
      public:
        GeometryLoader() noexcept = default;
        ~GeometryLoader() noexcept = default;

        static std::optional<GeometryData> load(const std::string& filename);
    };

}