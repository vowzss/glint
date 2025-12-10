#include "glint/graphics/backend/buffer/GeometryBuffer.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/scene/World.h"
#include "glint/scene/components/GeometryComponent.h"

using namespace glint::engine::core;
using namespace glint::engine::scene;

namespace glint::engine::graphics {
    SceneLayer::SceneLayer(const scene::World& world) : m_world(world) {};
    SceneLayer::~SceneLayer() = default;

    void SceneLayer::render(float deltaTime, const LayerRenderInfo& info) {
        const auto& entities = m_world.entities();

        vkCmdBindPipeline(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
        vkCmdBindDescriptorSets(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &info.cameraSet, 0, nullptr);
        vkCmdBindDescriptorSets(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 1, 1, &info.entitySet, 0, nullptr);

        VkDeviceSize offsets[] = {0};
        for (size_t i = 0; i < entities.size(); ++i) {
            const auto& entity = entities[i];

            const auto geometry = m_world.resolve<GeometryBuffer>(entity.geometry->handle());
            if (geometry == nullptr) continue;

            vkCmdBindVertexBuffers(info.commands, 0, 1, &geometry->m_vertices.m_handle, offsets);
            vkCmdBindIndexBuffer(info.commands, geometry->m_indices.m_handle, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(info.commands, geometry->m_indices.size(), 1, 0, 0, 0);
        }
    }
}