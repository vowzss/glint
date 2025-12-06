#pragma once

#include <vector>

#include "glint/graphics/models/Vertex.h"

namespace glint::engine::graphics {
    namespace backend {
        struct BufferData;
    }

    namespace models {
        struct Vertex;
    }
}

namespace glint::engine::scene::components {

    struct MeshCreateInfo {
        // MeshHandle handle;

        std::vector<graphics::models::Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct MeshComponent {
      public:
        graphics::backend::BufferData* vertices;
        graphics::backend::BufferData* indices;

      public:
        MeshComponent() = delete;
        MeshComponent(const MeshCreateInfo& info) : vertices(info.vertices), indices(info.indices) {}
    };

}