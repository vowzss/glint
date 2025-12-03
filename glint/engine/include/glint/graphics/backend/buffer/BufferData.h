#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {
    namespace backend {
        struct DeviceContext;

        struct BufferDataInfo {
            const void* data;
            VkDeviceSize size;
            VkDeviceSize offset;

            VkBufferUsageFlags usage;
            VkMemoryPropertyFlags properties;

          public:
            BufferDataInfo() = default;
        };

        struct BufferData {
            VkDevice device = {};

            VkBuffer value = {};
            VkDeviceSize size;
            VkDeviceMemory memory = {};

            void* data = {};

          public:
            BufferData() = delete;
            BufferData(const DeviceContext& devices, const BufferDataInfo& info);

            ~BufferData();

            void copy(const void* srcData, VkDeviceSize srcSize, VkDeviceSize srcOffset);
        };
    }
}