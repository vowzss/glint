#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {
    namespace backend {
        struct DeviceContext;

        struct BufferDataInfo {
            VkDeviceSize size;
            VkBufferUsageFlags usage;
            VkMemoryPropertyFlags properties;

            const void* data;

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

            void copy(const void* srcData, VkDeviceSize srcSize, VkDeviceSize srcOffset);
        };
    }
}