#include <cassert>
#include <cstring>
#include <stdexcept>

#include "glint/graphics/backend/VkHelpers.h"
#include "glint/graphics/backend/buffer/BufferData.h"
#include "glint/graphics/backend/device/Devices.h"

namespace glint::engine::graphics {

    BufferData::BufferData(const Devices& devices, const BufferCreateInfo& info) : m_device(devices.logical) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = info.size;
        bufferInfo.usage = info.usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_value) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to create buffer!");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(m_device, m_value, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(devices.physical, memoryRequirements.memoryTypeBits, info.properties);

        if (vkAllocateMemory(m_device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to allocate buffer memory!");
        }

        if (vkBindBufferMemory(m_device, m_value, m_memory, 0) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to bind buffer memory!");
        }

        m_size = info.size;

        bool isHostVisible = info.properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        if (isHostVisible && vkMapMemory(m_device, m_memory, 0, m_size, 0, &m_mapped) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to map buffer memory!");
        }

        if (info.data && m_mapped) {
            memcpy(m_mapped, info.data, m_size);
        }
    }

    BufferData::~BufferData() noexcept {
        if (m_mapped != nullptr) {
            vkUnmapMemory(m_device, m_memory);
            m_mapped = nullptr;
            m_size = 0;
        }

        if (m_value != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_device, m_value, nullptr);
            m_value = nullptr;
        }

        if (m_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_device, m_memory, nullptr);
            m_memory = nullptr;
        }
    }

    bool BufferData::update(VkDeviceSize size, const void* data, VkDeviceSize offset) {
        assert(m_mapped != nullptr && "Vulkan | Cannot update non-host-visible buffer directly");

        if (offset + m_size > size) {
            return false;
        }

        if (data != nullptr) {
            memcpy(static_cast<uint8_t*>(this->m_mapped) + offset, data, size);
        }

        return true;
    }
}
