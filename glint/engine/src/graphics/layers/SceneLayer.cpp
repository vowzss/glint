#include <vector>

#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/device/DeviceContext.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/graphics/models/Mesh.h"
#include "glint/graphics/models/Vertex.h"
#include "glint/loaders/ModelLoader.h"
#include "glint/scene/Entity.h"

namespace glint::engine {
    using namespace loaders;
}

namespace glint::engine {
    using namespace scene;
}

namespace glint::engine::graphics {
    using namespace backend;
    using namespace models;

    namespace layers {
        SceneLayer::SceneLayer(const DeviceContext& devices, SceneLayerCreateInfo info_) : device(devices.logical), info(info_) {

            {
                std::vector<Vertex> vertices = {
                    {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
                    {{0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                };
                std::vector<uint32_t> indices = {0, 1, 2};

                entities.emplace_back(new Entity{
                    0, new Mesh{vertices, indices}
                });
            }

            BufferDataInfo triangleVertexBufferInfo = {};
            triangleVertexBufferInfo.data = entities[0]->getMesh().vertices.data();
            triangleVertexBufferInfo.size = sizeof(entities[0]->getMesh().vertices[0]) * entities[0]->getMesh().vertices.size();
            triangleVertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            triangleVertexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            triangleVertexBuffer = new BufferData(devices, triangleVertexBufferInfo);

            BufferDataInfo triangleIndexBufferInfo = {};
            triangleIndexBufferInfo.data = entities[0]->getMesh().indices.data();
            triangleIndexBufferInfo.size = sizeof(entities[0]->getMesh().indices[0]) * entities[0]->getMesh().indices.size();
            triangleIndexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            triangleIndexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            triangleIndexBuffer = new BufferData(devices, triangleIndexBufferInfo);

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

                entities.emplace_back(new Entity{
                    1, new Mesh{vertices, indices}
                });
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

            /*auto loader = new ModelLoader();
            auto mesh = loader->load("suzanne.obj");*/

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

            vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
        }

        SceneLayer::~SceneLayer() {
            vkDestroyBuffer(device, triangleVertexBuffer->value, nullptr);
            vkFreeMemory(device, triangleVertexBuffer->memory, nullptr);
            vkDestroyBuffer(device, triangleIndexBuffer->value, nullptr);
            vkFreeMemory(device, triangleIndexBuffer->memory, nullptr);

            vkDestroyBuffer(device, cubeVertexBuffer->value, nullptr);
            vkFreeMemory(device, cubeVertexBuffer->memory, nullptr);
            vkDestroyBuffer(device, cubeIndexBuffer->value, nullptr);
            vkFreeMemory(device, cubeIndexBuffer->memory, nullptr);
        }

        void SceneLayer::renderFrame(const FrameData& frame, VkCommandBuffer commands) {
            frame.update(info.camera);

            JPH::Mat44* models = reinterpret_cast<JPH::Mat44*>(entityBuffer->data);

            for (int i = 0; i < entities.size(); ++i) {
                entities[i]->lifetime += frame.deltaTime;
                models[i] = entities[i]->getModelMatrix();
            }

            entityBuffer->copy(models, sizeof(JPH::Mat44) * entityBuffer->size, 0);

            vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);

            vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &frame.cameraDescriptorSet, 0, nullptr);
            vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 1, 1, &frame.entityDescriptorSet, 0, nullptr);

            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(commands, 0, 1, &triangleVertexBuffer->value, offsets);
            vkCmdBindIndexBuffer(commands, triangleIndexBuffer->value, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commands, static_cast<uint32_t>(entities[0]->getMesh().indices.size()), 1, 0, 0, 0);

            vkCmdBindVertexBuffers(commands, 0, 1, &cubeVertexBuffer->value, offsets);
            vkCmdBindIndexBuffer(commands, cubeIndexBuffer->value, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commands, static_cast<uint32_t>(entities[1]->getMesh().indices.size()), 1, 0, 0, 0);
        }
    }
}