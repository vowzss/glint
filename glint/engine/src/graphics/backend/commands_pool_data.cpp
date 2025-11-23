#include "glint/graphics/backend/commands_pool_data.h"

#include <cstdint>
#include <stdexcept>

#include <sys/types.h>
#include <vulkan/vulkan_core.h>

#include "glint/graphics/backend/queue_family_support_details.h"

commands_pool_data::commands_pool_data(const VkDevice& device, const queue_family_support_details& family, size_t size) : device(device) {

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = family.index;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &value) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create command pool!");
    }

    buffers.resize(size);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = value;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());

    if (vkAllocateCommandBuffers(device, &allocInfo, buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to allocate command buffers!");
    }
}

commands_pool_data::~commands_pool_data() {
    if (!buffers.empty()) {
        vkFreeCommandBuffers(device, value, buffers.size(), buffers.data());
        buffers.clear();
    }

    if (value != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device, value, nullptr);
        value = VK_NULL_HANDLE;
    }
}

void commands_pool_data::begin(size_t idx) {
    vkResetCommandBuffer(buffers.at(idx), 0);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(buffers.at(idx), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to begin recording command buffer!");
    }
}

void commands_pool_data::end(size_t idx) {
    if (vkEndCommandBuffer(buffers.at(idx)) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to end recording command buffer!");
    }
}