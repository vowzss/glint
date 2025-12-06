#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/scene/components/GeometryComponent.h"

namespace glint::engine::scene::components {

    GeometryComponent::GeometryComponent(const Devices& devices, const GeometryData& data) {
        using namespace graphics::backend;

        vertices = std::make_shared<BufferData>(
            BufferData::vertex(devices, data.vertices.data(), data.vertices.size()));

        indices = std::make_shared<BufferData>(
            BufferData::index(devices, data.indices.data(), data.indices.size()));
    }
}