#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>

#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/device/DeviceContext.h"
#include "glint/scene/components/Camera.h"

namespace glint::engine::graphics {
    namespace backend {
        FrameData::FrameData(const DeviceContext& devices, const FrameDataCreateInfo& info) : device(devices.logical) {
            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailable);
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinished);

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            vkCreateFence(device, &fenceInfo, nullptr, &inFlight);

            // camera UBO
            {
                VkDescriptorSetAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = info.descriptorPool;
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts = &info.cameraDescriptorLayout;

                if (vkAllocateDescriptorSets(device, &allocInfo, &cameraDescriptorSet) != VK_SUCCESS) {
                    throw std::runtime_error("Vulkan | Failed to create camera descriptor!");
                }

                JPH::Mat44 projMatrix = info.camera.getProjectionMatrix();
                JPH::Mat44 viewMatrix = info.camera.getViewMatrix();
                JPH::Mat44 projViewMatrix = projMatrix * viewMatrix;
                JPH::Mat44 cameraMatrices[3] = {viewMatrix, projMatrix, projViewMatrix};

                BufferDataInfo dataInfo{};
                dataInfo.data = cameraMatrices;
                dataInfo.size = sizeof(JPH::Mat44) * 3;
                dataInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                dataInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
                cameraBuffer = std::make_unique<BufferData>(devices, dataInfo);

                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = cameraBuffer->value;
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(JPH::Mat44) * 3;

                VkWriteDescriptorSet writeInfo{};
                writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeInfo.dstSet = cameraDescriptorSet;
                writeInfo.dstBinding = 0;
                writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                writeInfo.descriptorCount = 1;
                writeInfo.pBufferInfo = &bufferInfo;

                vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
            }

            // entity SSBO
            {
                VkDescriptorSetAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = info.descriptorPool;
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts = &info.entityDescriptorLayout;

                if (vkAllocateDescriptorSets(device, &allocInfo, &entityDescriptorSet) != VK_SUCCESS) {
                    throw std::runtime_error("Vulkan | Failed to create entity descriptor!");
                }
            }
        }

        FrameData::~FrameData() {
            if (inFlight != VK_NULL_HANDLE) {
                vkDestroyFence(device, inFlight, nullptr);
            }

            if (imageAvailable != VK_NULL_HANDLE) {
                vkDestroySemaphore(device, imageAvailable, nullptr);
            }

            if (renderFinished != VK_NULL_HANDLE) {
                vkDestroySemaphore(device, renderFinished, nullptr);
            }
        }

        void FrameData::update(const scene::components::Camera& camera) const {
            JPH::Mat44 projMatrix = camera.getProjectionMatrix();
            JPH::Mat44 viewMatrix = camera.getViewMatrix();
            JPH::Mat44 projViewMatrix = projMatrix * viewMatrix;
            JPH::Mat44 cameraMatrices[3] = {viewMatrix, projMatrix, projViewMatrix};

            cameraBuffer->copy(cameraMatrices, sizeof(JPH::Mat44) * 3, 0);
        }
    }
}