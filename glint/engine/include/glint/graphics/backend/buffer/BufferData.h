#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct Devices;

    struct BufferCreateInfo {
        const void* data = nullptr;
        VkDeviceSize size = 0;

        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };

    struct BufferData {
        // --- GPU handles ---
        const VkDevice m_device = nullptr;
        VkBuffer m_handle = nullptr;
        VkDeviceMemory m_memory = nullptr;

      protected:
        // --- GPU info ---
        void* m_mapped = nullptr;
        VkDeviceSize m_size = 0;

      protected:
        BufferData(const Devices& devices, const BufferCreateInfo& info);

      public:
        virtual ~BufferData() noexcept;

        BufferData(const BufferData&) = delete;
        BufferData& operator=(const BufferData&) = delete;

        BufferData(BufferData&& other) noexcept;
        BufferData& operator=(BufferData&& other) noexcept;

        bool update(VkDeviceSize size, const void* data = nullptr, VkDeviceSize offset = 0);

        // --- getters ---
        inline const void* data() const noexcept {
            return m_mapped;
        }

        inline const VkDeviceSize& size() const noexcept {
            return m_size;
        }
    };

}