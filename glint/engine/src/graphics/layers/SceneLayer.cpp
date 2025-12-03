#include <vector>

#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/device/DeviceContext.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/graphics/models/Vertex.h"
#include "glint/loaders/ModelLoader.h"

namespace glint::engine {
    using namespace loaders;

}

namespace glint::engine::graphics {
    using namespace backend;
    using namespace models;

    namespace layers {
        SceneLayer::SceneLayer(const DeviceContext& devices, SceneLayerInfo info_) : device(devices.logical), info(info_) {

            {
                std::vector<Vertex> triangleVertices = {
                    {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
                    {{0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                };
                BufferDataInfo triangleVertexBufferInfo = {};
                triangleVertexBufferInfo.data = triangleVertices.data();
                triangleVertexBufferInfo.size = sizeof(triangleVertices[0]) * triangleVertices.size();
                triangleVertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                triangleVertexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                triangleVertexBuffer = new BufferData(devices, triangleVertexBufferInfo);
            }
            {
                std::vector<uint32_t> triangleIndices = {0, 1, 2};
                BufferDataInfo triangleIndexBufferInfo = {};
                triangleIndexBufferInfo.data = triangleIndices.data();
                triangleIndexBufferInfo.size = sizeof(triangleIndices[0]) * triangleIndices.size();
                triangleIndexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
                triangleIndexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                triangleIndexBuffer = new BufferData(devices, triangleIndexBufferInfo);
            }

            {
                std::vector<Vertex> cubeVertices = {
                    {{0.5f, -0.5f, 0.5f},   {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {1.0f, 0.0f}},
                    {{0.5f, 0.5f, 0.5f},    {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {1.0f, 1.0f}},
                    {{-0.5f, 0.5f, 0.5f},   {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},  {0.0f, 1.0f}},
                    {{-0.5f, -0.5f, 0.5f},  {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f}},

                    {{0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
                    {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
                    {{-0.5f, 0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
                    {{0.5f, 0.5f, -0.5f},   {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
                };
                BufferDataInfo cubeVertexBufferInfo = {};
                cubeVertexBufferInfo.data = cubeVertices.data();
                cubeVertexBufferInfo.size = sizeof(cubeVertices[0]) * cubeVertices.size();
                cubeVertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                cubeVertexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                cubeVertexBuffer = new BufferData(devices, cubeVertexBufferInfo);
            }
            {
                std::vector<uint32_t> cubeIndices = {
                    0, 1, 2, 2, 3, 0, // Front
                    0, 1, 7, 7, 4, 0, // Right
                    4, 7, 6, 6, 5, 4, // Back
                    3, 2, 6, 6, 5, 3, // Left
                    1, 2, 6, 6, 7, 1, // Top
                    0, 3, 5, 5, 4, 0  // Bottom
                };
                BufferDataInfo cubeIndexBufferInfo = {};
                cubeIndexBufferInfo.data = cubeIndices.data();
                cubeIndexBufferInfo.size = sizeof(cubeIndices[0]) * cubeIndices.size();
                cubeIndexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
                cubeIndexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                cubeIndexBuffer = new BufferData(devices, cubeIndexBufferInfo);
            }

            auto loader = new ModelLoader();
            auto mesh = loader->load("suzanne.obj");
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
            frame.update(*info.camera);

            vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
            vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipelineLayout, 0, 1, &frame.cameraDescriptorSet, 0, nullptr);

            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(commands, 0, 1, &triangleVertexBuffer->value, offsets);
            vkCmdBindIndexBuffer(commands, triangleIndexBuffer->value, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commands, static_cast<uint32_t>(triangleIndexBuffer->size), 1, 0, 0, 0);

            vkCmdBindVertexBuffers(commands, 0, 1, &cubeVertexBuffer->value, offsets);
            vkCmdBindIndexBuffer(commands, cubeIndexBuffer->value, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commands, static_cast<uint32_t>(cubeIndexBuffer->size), 1, 0, 0, 0);
        }
    }
}