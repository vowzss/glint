#include "glint/graphics/backend/buffer/GeometryBuffer.h"
#include "glint/graphics/models/GeometryData.h"

namespace glint::engine::graphics {

    GeometryBuffer::GeometryBuffer(const Devices& devices, const GeometryData& data) {
        const auto& v = data.vertices;
        vertices = std::make_unique<VertexBuffer>(devices, v.size(), v.data());

        const auto& i = data.indices;
        indices = std::make_unique<IndexBuffer>(devices, i.size(), i.data());
    }

}