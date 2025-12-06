#include <vector>

#include "glint/core/EcsManager.h"
#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/device/DeviceContext.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/graphics/models/Vertex.h"
#include "glint/loaders/ModelLoader.h"
#include "glint/scene/components/MeshComponent.h"
#include "glint/scene/components/Transform.h"

namespace glint::engine {
    using namespace loaders;
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

    SceneLayer::SceneLayer(const DeviceContext& devices, SceneLayerCreateInfo info_) : device(devices.logical), info(info_) {

        /*
        {
            std::vector<Vertex> vertices = {
                {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
                {{0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            };
            std::vector<uint32_t> indices = {0, 1, 2};
        }

        triangleVertexBuffer = BufferData::vertex(devices, entities[0]->getMesh().vertices.data(),
            sizeof(entities[0]->getMesh().vertices[0]) * entities[0]->getMesh().vertices.size());

        triangleIndexBuffer = BufferData::index(devices, entities[0]->getMesh().indices.data(),
            sizeof(entities[0]->getMesh().indices[0]) * entities[0]->getMesh().indices.size());

        {
            std::vector<Vertex> vertices = {
                {{0.5f, -0.5f, 0.5f},   {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {1.0f, 0.0f}},
                {{0.5f, 0.5f, 0.5f},    {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {1.0f, 1.0f}},
                {{-0.5f, 0.5f, 0.5f},   {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},  {0.0f, 1.0f}},
                {{-0.5f, -0.5f, 0.5f},  {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f}},

                {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
                {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
                {{-0.5f, 0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
                {{0.5f, 0.5f, -0.5f},   {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            };
            std::vector<uint32_t> indices = {
                0, 1, 2, 2, 3, 0, // Front
                0, 1, 7, 7, 4, 0, // Right
                4, 7, 6, 6, 5, 4, // Back
                3, 2, 6, 6, 5, 3, // Left
                1, 2, 6, 6, 7, 1, // Top
                0, 3, 5, 5, 4, 0  // Bottom
            };
        }

        BufferDataInfo cubeVertexBufferInfo = {};
        cubeVertexBufferInfo.data = entities[1]->getMesh().vertices.data();
        cubeVertexBufferInfo.size = sizeof(entities[1]->getMesh().vertices[0]) * entities[1]->getMesh().vertices.size();
        cubeVertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        cubeVertexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        cubeVertexBuffer = new BufferData(devices, cubeVertexBufferInfo);

        BufferDataInfo cubeIndexBufferInfo = {};
        cubeIndexBufferInfo.data = entities[1]->getMesh().indices.data();
        cubeIndexBufferInfo.size = sizeof(entities[1]->getMesh().indices[0]) * entities[1]->getMesh().indices.size();
        cubeIndexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        cubeIndexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        cubeIndexBuffer = new BufferData(devices, cubeIndexBufferInfo);

        auto loader = new ModelLoader();
        auto mesh = loader->load("suzanne.obj");

        std::vector<JPH::Mat44> entityMatrices(entities.size());

        BufferDataInfo entityBufferData = {};
        entityBufferData.size = sizeof(JPH::Mat44) * entities.size();
        entityBufferData.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        entityBufferData.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        entityBufferData.data = nullptr;
        entityBuffer = new BufferData(devices, entityBufferData);

        VkDescriptorBufferInfo entityBufferInfo = {};
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

        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);*/
    }

    void SceneLayer::render(float deltaTime, const VkCommandBuffer& commands) {

        JPH::Mat44* models = reinterpret_cast<JPH::Mat44*>(entityBuffer->data);

        auto& transforms = info.ecs.getComponentStorage<Transform>();
        auto& meshes = info.ecs.getComponentStorage<MeshComponent>();

        for (size_t i = 0; i < transforms.data.size(); ++i) {
            uint32_t entityId = transforms.owners[i];
            if (!meshes.contains(EntityHandle{entityId})) continue;

            models[i] = transforms.data[i].toMatrix();
        }

        entityBuffer->copy(models, sizeof(JPH::Mat44) * entityBuffer->size, 0);

        vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);

        vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &frame.cameraDescriptorSet, 0, nullptr);
        vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 1, 1, &frame.entityDescriptorSet, 0, nullptr);

        VkDeviceSize offsets[] = {0};
        for (size_t i = 0; i < meshes.data.size(); ++i) {
            vkCmdBindVertexBuffers(commands, 0, 1, meshes[i].vertices->value, offsets);
            vkCmdBindIndexBuffer(commands, meshes[i].indices->value, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commands, meshes[i].indices.size()), 1, 0, 0, 0);
        }
    }
}