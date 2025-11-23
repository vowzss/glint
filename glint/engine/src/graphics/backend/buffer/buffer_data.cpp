#include "glint/graphics/backend/buffer/buffer_data.h"

#include <cstring>
#include <stdexcept>

#include "glint/graphics/backend/buffer/buffer_data_info.h"
#include "glint/graphics/backend/device/device_context.h"
#include "glint/graphics/backend/vk_helpers.h"

namespace glint::engine::graphics::backend {
    buffer_data::buffer_data(const device_context& devices, const buffer_data_info& info) : device(devices.logical), size(info.size) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = info.usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &value) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, value, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(devices.physical, memRequirements.memoryTypeBits, info.properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to allocate buffer memory!");
        }

        if (vkBindBufferMemory(device, value, memory, 0) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to bind buffer memory!");
        }

        void* mapped = nullptr;
        if (vkMapMemory(device, memory, 0, size, 0, &mapped) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to map buffer memory!");
        }

        memcpy(mapped, info.data, size);
        vkUnmapMemory(device, memory);
    }

    buffer_data::~buffer_data() {
        if (value != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, value, nullptr);
        }

        if (memory != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, nullptr);
        }
    }
}