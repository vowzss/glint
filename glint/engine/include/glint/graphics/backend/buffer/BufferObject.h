#pragma once

#include "glint/graphics/backend/device/Devices.h"

#include "vulkan/vulkan_core.h"

namespace glint::engine::graphics {

    struct BufferInfo {
        const void* data = nullptr;
        VkDeviceSize size = 0;

        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };

    struct BufferObject {
        // --- GPU handles ---
        const VkDevice m_device = nullptr;
        VkBuffer m_handle = nullptr;
        VkDeviceMemory m_memory = nullptr;

      protected:
        // --- GPU info ---
        void* m_mapped = nullptr;
        VkDeviceSize m_size = 0;

      protected:
        BufferObject(const Devices& devices, const BufferInfo& info);

      public:
        virtual ~BufferObject() noexcept;

        BufferObject(const BufferObject&) = delete;
        BufferObject& operator=(const BufferObject&) = delete;

        BufferObject(BufferObject&& other) noexcept;
        BufferObject& operator=(BufferObject&& other) noexcept;

        // --- methods ---
        bool update(VkDeviceSize size, const void* data = nullptr, VkDeviceSize offset = 0);

        // --- getters ---
        inline const void* data() const noexcept {
            return m_mapped;
        }

        inline const VkDeviceSize& size() const noexcept {
            return m_size;
        }

        inline const VkBuffer& handle() const noexcept {
            return m_handle;
        }
    };

}