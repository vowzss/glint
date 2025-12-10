#include "glint/graphics/backend/buffer/GeometryBuffer.h"
#include "glint/graphics/models/GeometryData.h"

namespace glint::engine::graphics {

    GeometryBuffer::GeometryBuffer(const Devices& devices, const GeometryData& data)
        : m_vertices(VertexBuffer{devices, static_cast<VkDeviceSize>(data.m_vertices.size()), data.m_vertices.data()}),
          m_indices(IndexBuffer{devices, static_cast<VkDeviceSize>(data.m_indices.size()), data.m_indices.data()}) {
    }

}