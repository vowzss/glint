#include <vector>

#include "glint/core/ecs/EntityManager.h"
#include "glint/core/ecs/World.h"
#include "glint/core/ecs/components/GeometryComponent.h"
#include "glint/core/ecs/components/TransformComponent.h"
#include "glint/graphics/backend/FrameObject.h"
#include "glint/graphics/backend/buffer/GeometryBufferObject.h"
#include "glint/graphics/backend/buffer/IndexBufferObject.h"
#include "glint/graphics/backend/buffer/VertexBufferObject.h"
#include "glint/graphics/layers/SceneLayer.h"

#include "Jolt/Math/Mat44.h"

using namespace glint::engine::core;

namespace glint::engine::graphics {
    SceneLayer::SceneLayer(const World& world) : m_world(world) {};
    SceneLayer::~SceneLayer() = default;

    void SceneLayer::render(float dt, FrameObject& frame) {
        const FrameRecordContext& context = frame.context();

        vkCmdBindPipeline(context.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline);
        vkCmdBindDescriptorSets(context.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipelineLayout, 0, 1, &frame.camera().set, 0, nullptr);
        vkCmdBindDescriptorSets(context.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipelineLayout, 1, 1, &frame.entity().set, 0, nullptr);

        const std::vector<EntityView>& entities = m_world.entities();

        std::vector<JPH::Mat44> transforms = {};
        transforms.resize(entities.size());
        for (const EntityView& entity : entities) {
            transforms.emplace_back(entity.transform->matrix());
        }

        frame.entity().buffer.update(transforms.size(), transforms.data());

        VkDeviceSize offsets[] = {0};
        for (const EntityView& entity : entities) {
            if (entity.geometry == nullptr) continue;

            const GeometryBufferObject* geometry = m_world.resolve<GeometryBufferObject>(entity.geometry->handle());
            if (geometry == nullptr) continue;

            vkCmdBindVertexBuffers(context.commands, 0, 1, &geometry->m_vertices->m_handle, offsets);
            vkCmdBindIndexBuffer(context.commands, geometry->m_indices->m_handle, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(context.commands, geometry->m_indices->size(), 1, 0, 0, 0);
        }
    }
}