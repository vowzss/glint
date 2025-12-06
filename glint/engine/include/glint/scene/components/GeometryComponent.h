#pragma once

#include <vector>

#include "glint/graphics/models/Vertex.h"

namespace glint::engine::graphics {
    namespace backend {
        struct DeviceContext;
        struct BufferData;
    }

    namespace models {
        struct Vertex;
    }
}

namespace glint::engine::scene::components {

    struct GeometryData {
      private:
        using vertices_t = std::vector<graphics::models::Vertex>;
        using indices_t = std::vector<uint32_t>;

      public:
        // MeshHandle handle;
        vertices_t vertices;
        indices_t indices;

      public:
        GeometryData() = default;
        GeometryData(const GeometryData&) = delete;
        GeometryData(GeometryData&&) noexcept = default;

        GeometryData(vertices_t vertices_, indices_t indices_)
            : vertices(std::move(vertices_)), indices(std::move(indices_)) {
        }

        GeometryData& operator=(const GeometryData&) = delete;
        GeometryData& operator=(GeometryData&&) noexcept = default;
    };

    struct GeometryComponent {
      private:
        using Devices = graphics::backend::DeviceContext;
        using BufferDataPtr = std::shared_ptr<graphics::backend::BufferData>;

      public:
        BufferDataPtr vertices;
        BufferDataPtr indices;

      public:
        GeometryComponent() = delete;
        GeometryComponent(const Devices& devices, const GeometryData& data);
    };

}