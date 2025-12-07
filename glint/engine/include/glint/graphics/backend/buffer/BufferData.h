#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct DeviceHandles;

    struct BufferCreateInfo {
        const void* data;
        VkDeviceSize size;

        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };

    struct BufferData {
        // --- GPU handles ---
        VkDevice device = nullptr;
        VkBuffer value = nullptr;
        VkDeviceMemory memory = nullptr;

      protected:
        // --- GPU info ---
        void* mapped = nullptr;
        VkDeviceSize size = 0;

      protected:
        BufferData() = default;

      public:
        virtual ~BufferData() noexcept;

        BufferData(const BufferData&) = delete;
        BufferData& operator=(const BufferData&) = delete;

        BufferData(BufferData&& other) = delete;
        BufferData& operator=(BufferData&& other) = delete;

      protected:
        void init(const DeviceHandles& devices, const BufferCreateInfo& info);

      public:
        void update(const void* data, VkDeviceSize size_, VkDeviceSize offset = 0);

        inline const void* getData() const {
            return mapped;
        }
        inline VkDeviceSize getSize() const {
            return size;
        }
    };

}