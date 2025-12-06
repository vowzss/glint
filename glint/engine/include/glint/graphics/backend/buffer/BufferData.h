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
        VkDevice device = nullptr;

        void* data = nullptr;
        VkDeviceSize size;

        VkBuffer value = nullptr;
        VkDeviceMemory memory = nullptr;

      public:
        BufferData() = delete;
        BufferData(const DeviceContext& devices, const BufferCreateInfo& info);

        ~BufferData();

        // --- factories ---
        static BufferData vertex(const DeviceContext& devices, const void* data, VkDeviceSize size);
        static BufferData index(const DeviceContext& devices, const void* data, VkDeviceSize size);

        // --- methods ---
        void copy(const void* srcData, VkDeviceSize srcSize, VkDeviceSize srcOffset);
    };

}