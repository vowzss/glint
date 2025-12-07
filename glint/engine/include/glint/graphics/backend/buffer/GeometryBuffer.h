#pragma once

#include <memory>

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace glint::engine::graphics::models {
    struct GeometryData;
}

namespace glint::engine::graphics::backend {

    struct GeometryBuffer {
      public:
        std::unique_ptr<VertexBuffer> vertices;
        std::unique_ptr<IndexBuffer> indices;

      public:
        GeometryBuffer() = delete;
        GeometryBuffer(const DeviceContext& devices, const models::GeometryData& data);
    };
}