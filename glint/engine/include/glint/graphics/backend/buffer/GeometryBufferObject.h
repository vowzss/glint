#pragma once

#include <memory>

#include "glint/graphics/backend/device/Devices.h"

namespace glint::engine::graphics {
    struct VertexBufferObject;
    struct IndexBufferObject;
    class GeometryData;

    struct GeometryBufferObject {
        std::shared_ptr<VertexBufferObject> m_vertices;
        std::shared_ptr<IndexBufferObject> m_indices;

      public:
        GeometryBufferObject() = delete;
        GeometryBufferObject(const Devices& devices, const GeometryData& data);
    };
}