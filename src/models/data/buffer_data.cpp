#include "glint/models/data/buffer_data.h"

#include <cstring>
#include <stdexcept>

#include "glint/misc/vk_helpers.h"
#include "glint/models/config/device_context.h"
#include "glint/models/misc/buffer_data_info.h"

using namespace glint;

buffer_data::buffer_data(const device_context& devices, const buffer_data_info& info)
    : device(devices.logical), size(info.size) {
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = info.usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &value) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, value, &memRequirements);

    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = helpers::findMemoryType(devices.physical, memRequirements.memoryTypeBits, info.properties),
    };

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