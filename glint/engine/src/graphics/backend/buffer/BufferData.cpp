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

        if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to create buffer!");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(m_device, m_handle, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(devices.physical, memoryRequirements.memoryTypeBits, info.properties);

        if (vkAllocateMemory(m_device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to allocate buffer memory!");
        }

        if (vkBindBufferMemory(m_device, m_handle, m_memory, 0) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to bind buffer memory!");
        }

        m_size = info.size;

        if (info.properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            if (vkMapMemory(m_device, m_memory, 0, m_size, 0, &m_mapped) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to map buffer memory!");
            }

            if (info.data) {
                std::memcpy(m_mapped, info.data, m_size);
            } else if (m_size > 0) {
                std::memset(m_mapped, 0, m_size);
            }
        }
    }

    BufferData::~BufferData() noexcept {
        if (m_mapped != nullptr) {
            vkUnmapMemory(m_device, m_memory);
            m_mapped = nullptr;
        }

        if (m_handle != nullptr) {
            vkDestroyBuffer(m_device, m_handle, nullptr);
            m_handle = nullptr;
        }

        if (m_memory != nullptr) {
            vkFreeMemory(m_device, m_memory, nullptr);
            m_memory = nullptr;
        }

        m_size = 0;
    }

    BufferData::BufferData(BufferData&& other) noexcept
        : m_device(other.m_device), m_handle(other.m_handle), m_memory(other.m_memory), m_mapped(other.m_mapped), m_size(other.m_size) {
        other.m_handle = nullptr;
        other.m_memory = nullptr;
        other.m_mapped = nullptr;
        other.m_size = 0;
    }

    BufferData& BufferData::operator=(BufferData&& other) noexcept {
        if (this != &other) {
            if (m_handle != nullptr) {
                if (m_mapped) vkUnmapMemory(m_device, m_memory);
                vkDestroyBuffer(m_device, m_handle, nullptr);
                vkFreeMemory(m_device, m_memory, nullptr);
            }

            m_handle = other.m_handle;
            m_memory = other.m_memory;
            m_mapped = other.m_mapped;
            m_size = other.m_size;

            other.m_handle = nullptr;
            other.m_memory = nullptr;
            other.m_mapped = nullptr;
            other.m_size = 0;
        }

        return *this;
    }

    bool BufferData::update(VkDeviceSize size, const void* data, VkDeviceSize offset) {
        if (m_mapped == nullptr) {
            return false;
        }

        if (offset + size > m_size) {
            return false;
        }

        if (data != nullptr) {
            memcpy(static_cast<uint8_t*>(m_mapped) + offset, data, size);
        }

        return true;
    }
}
