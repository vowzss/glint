#include <memory>

#include "glint/graphics/assets/Geometry.h"
#include "glint/graphics/backend/buffer/GeometryBufferObject.h"
#include "glint/graphics/backend/buffer/IndexBufferObject.h"
#include "glint/graphics/backend/buffer/VertexBufferObject.h"

namespace glint::engine::graphics {

    GeometryBufferObject::GeometryBufferObject(const Devices& devices, const Geometry& data) {
        const auto& vertices = data.vertices();
        const auto& indices = data.indices();

        m_vertices = std::make_unique<VertexBufferObject>(devices, static_cast<VkDeviceSize>(vertices.size()), vertices.data());
        m_indices = std::make_unique<IndexBufferObject>(devices, static_cast<VkDeviceSize>(indices.size()), indices.data());
    }
}