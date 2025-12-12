#include <cstdint>
#include <stdexcept>

#include "glint/graphics/backend/CommandsPoolData.h"
#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"

namespace glint::engine::graphics {

    CommandsPoolObject::CommandsPoolObject(const VkDevice& device, const QueueFamilySupportDetails& family) : m_device(device) {

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = family.index;

        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create command pool!");
        }

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_handle;
        allocInfo.commandBufferCount = static_cast<uint32_t>(family.count);

        m_buffers.reserve(family.count);
        if (vkAllocateCommandBuffers(m_device, &allocInfo, m_buffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to allocate command buffers!");
        }
    }

    CommandsPoolObject::~CommandsPoolObject() {
        vkFreeCommandBuffers(m_device, m_handle, static_cast<uint32_t>(m_buffers.size()), m_buffers.data());
        m_buffers.clear();

        if (m_handle != nullptr) {
            vkDestroyCommandPool(m_device, m_handle, nullptr);
            m_handle = nullptr;
        }
    }

    void CommandsPoolObject::begin(size_t idx) {
        const VkCommandBuffer& handle = m_buffers[idx];
        vkResetCommandBuffer(handle, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;

        if (vkBeginCommandBuffer(handle, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to begin command buffer!");
        }
    }

    void CommandsPoolObject::end(size_t idx) {
        const VkCommandBuffer& handle = m_buffers[idx];
        if (vkEndCommandBuffer(handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to end command buffer!");
        }
    }

    VkCommandBuffer CommandsPoolObject::beginSingle() const {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_handle;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer handle;
        if (vkAllocateCommandBuffers(m_device, &allocInfo, &handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to allocate single command buffer!");
        }

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (vkBeginCommandBuffer(handle, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to begin single command buffer!");
        }

        return handle;
    }

    void CommandsPoolObject::endSingle(VkCommandBuffer commands, VkQueue queue) const {
        if (vkEndCommandBuffer(commands) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to end single command buffer!");
        }

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pCommandBuffers = &commands;
        submitInfo.commandBufferCount = 1;

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to submit single command buffer!");
        }

        vkQueueWaitIdle(queue);
        vkFreeCommandBuffers(m_device, m_handle, 1, &commands);
    }
}