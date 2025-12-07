#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct DeviceContext;

    struct BufferCreateInfo {
        const void* data;
        VkDeviceSize size;

        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;

      public:
        BufferCreateInfo() = default;
    };

    struct BufferData {
        VkDevice device;

        void* mapped;
        VkDeviceSize size;

        VkBuffer value;
        VkDeviceMemory memory;

      protected:
        BufferData() = default;
        virtual ~BufferData() noexcept;

        // --- methods ---
        void init(const DeviceContext& devices, const BufferCreateInfo& info);

      public:
        BufferData(const BufferData&) = delete;
        BufferData(BufferData&& other) = delete;

        // --- methods ---
        void copy(const void* data, VkDeviceSize size_, VkDeviceSize offset = 0);

        // --- operators ---
        BufferData& operator=(const BufferData&) = delete;
        BufferData& operator=(BufferData&& other) noexcept;
    };

}