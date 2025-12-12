#include "glint/core/ecs/World.h"
#include "glint/core/ecs/components/GeometryComponent.h"
#include "glint/graphics/backend/buffer/GeometryBufferObject.h"
#include "glint/graphics/backend/buffer/IndexBufferObject.h"
#include "glint/graphics/backend/buffer/VertexBufferObject.h"
#include "glint/graphics/layers/SceneLayer.h"

using namespace glint::engine::core;

namespace glint::engine::graphics {
    SceneLayer::SceneLayer(const World& world) : m_world(world) {};
    SceneLayer::~SceneLayer() = default;

    void SceneLayer::render(float deltaTime, const LayerRenderInfo& info) {
        vkCmdBindPipeline(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
        vkCmdBindDescriptorSets(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &info.cameraSet, 0, nullptr);
        vkCmdBindDescriptorSets(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 1, 1, &info.entitySet, 0, nullptr);

        VkDeviceSize offsets[] = {0};
        for (const EntityView& entity : *info.entities) {
            if (entity.geometry == nullptr) continue;

            const GeometryBufferObject* geometry = m_world.resolve<GeometryBufferObject>(entity.geometry->handle());
            if (geometry == nullptr) continue;

            vkCmdBindVertexBuffers(info.commands, 0, 1, &geometry->m_vertices->m_handle, offsets);
            vkCmdBindIndexBuffer(info.commands, geometry->m_indices->m_handle, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(info.commands, geometry->m_indices->size(), 1, 0, 0, 0);
        }
    }
}