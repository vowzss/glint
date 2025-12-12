#include <cstddef>

#include "glint/core/ecs/World.h"
#include "glint/core/systems/CameraSystem.h"
#include "glint/graphics/backend/FrameObject.h"
#include "glint/graphics/backend/device/Devices.h"
#include "glint/graphics/layers/RenderLayer.h"

#include "Jolt/Math/Mat44.h"

using namespace glint::engine::core;

namespace glint::engine::graphics {

    FrameObject::FrameObject(const Devices& devices, const FrameInfo& info)
        : m_device(devices.logical), m_camera{VK_NULL_HANDLE, UniformBufferObject(devices, CameraSnapshot::size())},
          m_entity{VK_NULL_HANDLE, StorageBufferObject(devices, sizeof(JPH::Mat44) * 10)} {
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

    FrameObject::~FrameObject() {
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

    void FrameObject::begin() {
    }

    void FrameObject::record(float dt, const FrameRecordContext& context) {
        m_context = &context;

        m_camera.buffer.update(m_context->camera.size(), m_context->camera.data());

        for (size_t i = 0; i < m_layers.size(); ++i) {
            RenderLayer& layer = m_layers[i].get();
            layer.render(dt, *this);
        }
    }

    void FrameObject::end() {
        m_context = nullptr;
    }

    void FrameObject::attach(RenderLayer& layer) noexcept {
        auto it = std::find_if(m_layers.begin(), m_layers.end(), [&layer](auto& ref) { return &ref.get() == &layer; });
        if (it != m_layers.end()) return;

        m_layers.push_back(std::ref(layer));
    }

    void FrameObject::detach(RenderLayer& layer) noexcept {
        m_layers.erase(std::remove_if(m_layers.begin(), m_layers.end(), [&layer](auto& ref) { return &ref.get() == &layer; }), m_layers.end());
    }
}