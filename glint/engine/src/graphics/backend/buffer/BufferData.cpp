#include <cstring>
#include <stdexcept>

#include "glint/graphics/backend/VkHelpers.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/device/DeviceContext.h"

namespace glint::engine::graphics::backend {

    BufferData::BufferData(const DeviceContext& devices, const BufferCreateInfo& info) : device(devices.logical), size(info.size) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = info.usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &value) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create buffer!");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(device, value, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(devices.physical, memoryRequirements.memoryTypeBits, info.properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to allocate buffer memory!");
        }

        if (vkBindBufferMemory(device, value, memory, 0) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to bind buffer memory!");
        }

        bool isHostVisible = info.properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        if (isHostVisible && vkMapMemory(device, memory, 0, size, 0, &data) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to map buffer memory!");
        }

        if (info.data && data) {
            memcpy(data, info.data, size);
        }
    }

    BufferData::~BufferData() {
        if (data) {
            vkUnmapMemory(device, memory);
        }

        if (value != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, value, nullptr);
        }

        if (memory != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, nullptr);
        }
    }

    // --- factories ---
    BufferData BufferData::vertex(const DeviceContext& devices, const void* data, VkDeviceSize size) {
        BufferCreateInfo createInfo = {};
        createInfo.data = data;
        createInfo.size = size;
        createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        createInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        return BufferData(devices, createInfo);
    }

    BufferData BufferData::index(const DeviceContext& devices, const void* data, VkDeviceSize size) {
        BufferCreateInfo createInfo = {};
        createInfo.data = data;
        createInfo.size = size;
        createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        createInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        return BufferData(devices, createInfo);
    }

    // --- methods ---
    void BufferData::copy(const void* data, VkDeviceSize size, VkDeviceSize offset) {
        if (offset + size > this->size) return;
        memcpy(static_cast<uint8_t*>(this->data) + offset, data, size);
    }
}
