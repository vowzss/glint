#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {
    namespace backend {
        struct DeviceContext;

        struct BufferDataInfo {
            const void* data;
            VkDeviceSize size;

            VkBufferUsageFlags usage;
            VkMemoryPropertyFlags properties;

          public:
            BufferDataInfo() = default;
        };

        struct BufferData {
            VkDevice device = nullptr;

            VkBuffer value = nullptr;
            VkDeviceSize size;
            VkDeviceMemory memory = nullptr;

            void* data = nullptr;

          public:
            BufferData() = delete;
            BufferData(const DeviceContext& devices, const BufferDataInfo& info);

            ~BufferData();

            // --- factories ---
            static BufferData vertex(const DeviceContext& devices, const void* data, VkDeviceSize size);
            static BufferData index(const DeviceContext& devices, const void* data, VkDeviceSize size);

            void copy(const void* srcData, VkDeviceSize srcSize, VkDeviceSize srcOffset);
        };
    }
}