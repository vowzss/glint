#pragma once

#include <optional>
#include <string>

namespace glint::engine::scene::components {
    struct GeometryData;
}

namespace glint::engine::loaders {

    struct GeometryLoader {
      private:
        using GeometryData = scene::components::GeometryData;

      public:
        GeometryLoader() = default;
        ~GeometryLoader() = default;

      public:
        static std::optional<GeometryData> load(const std::string& filename);
    };

}