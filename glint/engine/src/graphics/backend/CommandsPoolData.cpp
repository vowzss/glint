#include <cstdint>
#include <stdexcept>

#include "glint/graphics/backend/CommandsPoolData.h"
#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"

namespace glint::engine::graphics {

    CommandsPoolData::CommandsPoolData(const VkDevice& device, const QueueFamilySupportDetails& family, size_t size) : device(device) {

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = family.index;

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create command pool!");
        }

        buffers.resize(size);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = handle;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());

        if (vkAllocateCommandBuffers(device, &allocInfo, buffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to allocate command buffers!");
        }
    }

    CommandsPoolData::~CommandsPoolData() {
        if (!buffers.empty()) {
            vkFreeCommandBuffers(device, handle, buffers.size(), buffers.data());
            buffers.clear();
        }

        if (handle != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device, handle, nullptr);
            handle = VK_NULL_HANDLE;
        }
    }

    void CommandsPoolData::begin(size_t idx) {
        vkResetCommandBuffer(buffers.at(idx), 0);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;

        if (vkBeginCommandBuffer(buffers.at(idx), &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to begin recording command buffer!");
        }
    }

    void CommandsPoolData::end(size_t idx) {
        if (vkEndCommandBuffer(buffers.at(idx)) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to end recording command buffer!");
        }
    }

}