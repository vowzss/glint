#include <cstring>
#include <stdexcept>

#include "glint/graphics/backend/VkHelpers.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/device/DeviceContext.h"

namespace glint::engine::graphics {
    namespace backend {
        BufferData::BufferData(const DeviceContext& devices, const BufferDataInfo& info) : device(devices.logical), size(info.size) {
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

        void BufferData::copy(const void* srcData, VkDeviceSize srcSize, VkDeviceSize srcOffset) {
            if (srcOffset + srcSize > size) return;

            memcpy(static_cast<uint8_t*>(data) + srcOffset, srcData, srcSize);
        }
    }
}