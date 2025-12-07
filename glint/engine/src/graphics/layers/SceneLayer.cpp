#include "glint/core/ecs/ComponentManager.h"
#include "glint/core/ecs/ComponentStorage.h"
#include "glint/graphics/backend/buffer/UniformBuffer.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/scene/World.h"
#include "glint/scene/components/GeometryComponent.h"
#include "glint/scene/components/Transform.h"

namespace glint::engine {
    using namespace core;
    using namespace scene;

    namespace graphics {
        using namespace backend;
        using namespace models;
    }

    namespace scene {
        using namespace components;
    }
}

namespace glint::engine::graphics::layers {
    SceneLayer::SceneLayer(const scene::World& world_) : world(world_) {};

    SceneLayer::~SceneLayer() = default;

    void SceneLayer::init(const backend::DeviceHandles& devices) {

        std::vector<JPH::Mat44> entities;

        auto& transforms = world.getStorage<Transform>();
        transforms.each([&](const Transform& transform) { entities.emplace_back(transform.toMatrix()); });

        entityBuffer = std::make_unique<UniformBuffer>(devices, entities.data(), sizeof(JPH::Mat44) * entities.size());

        /*VkDescriptorBufferInfo entityBufferInfo = {};
        entityBufferInfo.buffer = entityBuffer->value;
        entityBufferInfo.offset = 0;
        entityBufferInfo.range = sizeof(JPH::Mat44) * 3;

        VkWriteDescriptorSet writeInfo = {};
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.dstSet = entityDescriptorSet;
        writeInfo.dstBinding = 0;
        writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        writeInfo.descriptorCount = 1;
        writeInfo.pBufferInfo = &entityBufferInfo;

        vkUpdateDescriptorSets(devices.logical, 1, &writeInfo, 0, nullptr);*/
    }

    void SceneLayer::render(float deltaTime, const LayerRenderInfo& info) {
        const JPH::Mat44* models = reinterpret_cast<const JPH::Mat44*>(entityBuffer->getData());
        entityBuffer->update(models, sizeof(JPH::Mat44) * entityBuffer->getSize(), 0);

        const auto& transforms = world.getStorage<Transform>();
        const auto& geometries = world.getStorage<GeometryComponent>();

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