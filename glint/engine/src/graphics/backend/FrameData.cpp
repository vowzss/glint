#include "glint/core/systems/CameraSystem.h"
#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/buffer/StorageBuffer.h"
#include "glint/graphics/backend/buffer/UniformBuffer.h"
#include "glint/graphics/backend/device/Devices.h"
#include "glint/graphics/layers/RenderLayer.h"

namespace glint::engine::graphics {
    using namespace core;

    FrameData::FrameData(const Devices& devices, const FrameCreateInfo& info) : m_device(devices.logical) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &imageAvailable);
        vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &renderFinished);

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(m_device, &fenceInfo, nullptr, &inFlight);

        // camera uniform buffer
        {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = info.descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &info.cameraLayout;

            if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_cameraSet) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create camera descriptor!");
            }

            m_cameraBuffer = std::make_unique<UniformBuffer>(devices, CameraSnapshot::size());

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_cameraBuffer->m_value;
            bufferInfo.offset = 0;
            bufferInfo.range = VK_WHOLE_SIZE;

            VkWriteDescriptorSet writeInfo{};
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.dstSet = m_cameraSet;
            writeInfo.dstBinding = 0;
            writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeInfo.descriptorCount = 1;
            writeInfo.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(m_device, 1, &writeInfo, 0, nullptr);
        }

        // entity storage buffer
        {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = info.descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &info.entityLayout;

            if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_entitySet) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create entity descriptor!");
            }

            m_entityBuffer = std::make_unique<StorageBuffer>(devices, sizeof(JPH::Mat44) * 15000);

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_entityBuffer->m_value;
            bufferInfo.offset = 0;
            bufferInfo.range = VK_WHOLE_SIZE;

            VkWriteDescriptorSet writeInfo{};
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.dstSet = m_entitySet;
            writeInfo.dstBinding = 0;
            writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeInfo.descriptorCount = 1;
            writeInfo.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(m_device, 1, &writeInfo, 0, nullptr);
        }
    }

    FrameData::~FrameData() {
        m_cameraBuffer.reset();
        m_entityBuffer.reset();

        if (inFlight != VK_NULL_HANDLE) {
            vkDestroyFence(m_device, inFlight, nullptr);
        }

        if (imageAvailable != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_device, imageAvailable, nullptr);
        }

        if (renderFinished != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_device, renderFinished, nullptr);
        }
    }

    void FrameData::begin() const {
        // your Vulkan commands to begin frame
    }

    void FrameData::render(float deltaTime, const FrameRenderInfo& info) const {
        tick(deltaTime);

        m_cameraBuffer->update(info.camera.size(), info.camera.data());

        LayerRenderInfo layerInfo = {info.commands, info.pipeline, info.pipelineLayout, m_cameraSet, m_entitySet};
        for (int i = 0; i < m_layers.size(); ++i) {
            m_layers[i]->render(this->m_deltaTime, layerInfo);
        }
    }

    void FrameData::end() const {
    }

    void FrameData::attach(RenderLayer* layer) noexcept {
        if (std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end()) return;
        m_layers.push_back(layer);
    }

    void FrameData::detach(RenderLayer* layer) noexcept {
        m_layers.erase(std::remove(m_layers.begin(), m_layers.end(), layer), m_layers.end());
    }

}