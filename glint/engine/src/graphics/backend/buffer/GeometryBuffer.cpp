#include "glint/graphics/backend/buffer/GeometryBuffer.h"
#include "glint/graphics/models/GeometryData.h"

namespace glint::engine::graphics::backend {

    GeometryBuffer::GeometryBuffer(const DeviceHandles& devices, const models::GeometryData& data) {
        const auto& v = data.vertices;
        vertices = std::make_unique<VertexBuffer>(devices, v.data(), v.size());

        const auto& i = data.indices;
        indices = std::make_unique<IndexBuffer>(devices, i.data(), i.size());
    }

}