#pragma once

#include <memory>

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace glint::engine::graphics {
    struct GeometryData;

    struct GeometryBuffer {
      public:
        std::unique_ptr<VertexBuffer> vertices;
        std::unique_ptr<IndexBuffer> indices;

      public:
        GeometryBuffer() = delete;
        GeometryBuffer(const Devices& devices, const GeometryData& data);
    };
}