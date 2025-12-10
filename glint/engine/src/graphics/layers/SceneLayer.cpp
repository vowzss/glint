#include "glint/graphics/layers/SceneLayer.h"
#include "glint/scene/World.h"

using namespace glint::engine::core;
using namespace glint::engine::scene;

namespace glint::engine::graphics {
    SceneLayer::SceneLayer(const scene::World& world_) : world(world_) {};
    SceneLayer::~SceneLayer() = default;

    void SceneLayer::render(float deltaTime, const LayerRenderInfo& info) {
        /*const auto& transforms = world.query<Transform>();
        const auto& geometries = world.query<GeometryComponent>();*/

        /*for (size_t i = 0; i < transforms.data.size(); ++i) {
            uint32_t entityId = transforms.owners[i];
            if (!geometries.contains(entityId)) continue;

            models[i] = transforms.data[i].toMatrix();
        }*/

        vkCmdBindPipeline(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
        vkCmdBindDescriptorSets(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &info.cameraSet, 0, nullptr);
        vkCmdBindDescriptorSets(info.commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 1, 1, &info.entitySet, 0, nullptr);

        /*VkDeviceSize offsets[] = {0};
        for (size_t i = 0; i < geometries.data.size(); ++i) {
            vkCmdBindVertexBuffers(info.commands, 0, 1, geometries[i].vertices->value, offsets);
            vkCmdBindIndexBuffer(info.commands, geometries[i].indices->value, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(info.commands, geometries[i].indices.size(), 1, 0, 0, 0);
        }*/
    }
}