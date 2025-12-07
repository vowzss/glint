#include <utility>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>

#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/buffer/UniformBuffer.h"
#include "glint/graphics/backend/device/DeviceHandles.h"
#include "glint/graphics/layers/RenderLayer.h"
#include "glint/scene/components/Camera.h"

using namespace glint::engine::graphics::layers;

namespace glint::engine::graphics::backend {

    FrameData::FrameData(const DeviceHandles& devices, const FrameCreateInfo& info) : device(devices.logical) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailable);
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinished);

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(device, &fenceInfo, nullptr, &inFlight);

        // camera uniform buffer
        {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = info.descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &info.cameraLayout;

            if (vkAllocateDescriptorSets(device, &allocInfo, &cameraSet) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create camera descriptor!");
            }

            cameraBuffer = std::make_unique<UniformBuffer>(devices, info.camera.data(), info.camera.size());

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = cameraBuffer->value;
            bufferInfo.offset = 0;
            bufferInfo.range = info.camera.size();

            VkWriteDescriptorSet writeInfo{};
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.dstSet = cameraSet;
            writeInfo.dstBinding = 0;
            writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeInfo.descriptorCount = 1;
            writeInfo.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
        }

        // entity uniform buffer
        {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = info.descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &info.entityLayout;

            if (vkAllocateDescriptorSets(device, &allocInfo, &entitySet) != VK_SUCCESS) {
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

    void FrameData::begin() const {
        // your Vulkan commands to begin frame
    }

    void FrameData::render(float deltaTime, const FrameRenderInfo& info) const {
        this->deltaTime = deltaTime;

        cameraBuffer->update(info.camera.data(), info.camera.size());

        LayerRenderInfo layerInfo = {info.commands, info.pipeline, info.pipelineLayout, cameraSet, entitySet};
        for (int i = 0; i < layers.size(); ++i) {
            layers[i]->render(this->deltaTime, layerInfo);
        }
    }

    void FrameData::end() const {
    }

    void FrameData::attach(layers::RenderLayer* layer) {
        if (std::find(layers.begin(), layers.end(), layer) != layers.end()) return;
        layers.push_back(layer);
    }

    void FrameData::detach(layers::RenderLayer* layer) {
        layers.erase(std::remove(layers.begin(), layers.end(), layer), layers.end());
    }

}