#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace glint::engine::graphics {
    struct GeometryData;

    struct GeometryBuffer {
        VertexBuffer m_vertices;
        IndexBuffer m_indices;

      public:
        GeometryBuffer() = delete;
        GeometryBuffer(const Devices& devices, const GeometryData& data);
    };
}