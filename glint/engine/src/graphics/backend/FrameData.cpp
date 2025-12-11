#include <cstddef>

#include "glint/core/systems/CameraSystem.h"
#include "glint/graphics/backend/FrameData.h"
#include "glint/graphics/backend/device/Devices.h"
#include "glint/graphics/layers/RenderLayer.h"
#include "glint/scene/World.h"

using namespace glint::engine::core;
using namespace glint::engine::scene;

namespace glint::engine::graphics {

    FrameData::FrameData(const Devices& devices, const FrameCreateInfo& info)
        : m_device(devices.logical), m_camera{VK_NULL_HANDLE, UniformBuffer(devices, CameraSnapshot::size())},
          m_entity{VK_NULL_HANDLE, StorageBuffer(devices, sizeof(JPH::Mat44) * 100)} {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_ready);
        vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_done);

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(m_device, &fenceInfo, nullptr, &m_guard);

        // camera uniform buffer
        {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = info.descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &info.cameraLayout;

            if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_camera.set) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create camera descriptor!");
            }

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_camera.buffer.m_handle;
            bufferInfo.offset = 0;
            bufferInfo.range = VK_WHOLE_SIZE;

            VkWriteDescriptorSet writeInfo{};
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.dstSet = m_camera.set;
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

            if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_entity.set) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create entity descriptor!");
            }

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_entity.buffer.m_handle;
            bufferInfo.offset = 0;
            bufferInfo.range = VK_WHOLE_SIZE;

            VkWriteDescriptorSet writeInfo{};
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.dstSet = m_entity.set;
            writeInfo.dstBinding = 0;
            writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeInfo.descriptorCount = 1;
            writeInfo.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(m_device, 1, &writeInfo, 0, nullptr);
        }
    }

    FrameData::~FrameData() {
        if (m_guard != VK_NULL_HANDLE) {
            vkDestroyFence(m_device, m_guard, nullptr);
        }

        if (m_ready != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_device, m_ready, nullptr);
        }

        if (m_done != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_device, m_done, nullptr);
        }
    }

    void FrameData::begin() {
        // your Vulkan commands to begin frame
    }

    void FrameData::render(float deltaTime, const FrameRenderInfo& info) {
        m_deltaTime = deltaTime;

        m_camera.buffer.update(info.camera.size(), info.camera.data());

        const std::vector<EntityView> entities = {};
        const JPH::Mat44* models = reinterpret_cast<const JPH::Mat44*>(m_entity.buffer.data());
        m_entity.buffer.update(m_entity.buffer.size(), models);

        LayerRenderInfo renderInfo;
        renderInfo.commands = info.commands;
        renderInfo.pipeline = info.pipeline;
        renderInfo.pipelineLayout = info.pipelineLayout;
        renderInfo.cameraSet = m_camera.set;
        renderInfo.entitySet = m_entity.set;
        renderInfo.entities = &entities;

        for (size_t i = 0; i < m_layers.size(); ++i) {
            m_layers[i]->render(this->m_deltaTime, renderInfo);
        }
    }

    void FrameData::end() {
    }

    void FrameData::attach(RenderLayer* layer) noexcept {
        if (std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end()) return;
        m_layers.push_back(layer);
    }

    void FrameData::detach(RenderLayer* layer) noexcept {
        m_layers.erase(std::remove(m_layers.begin(), m_layers.end(), layer), m_layers.end());
    }

}