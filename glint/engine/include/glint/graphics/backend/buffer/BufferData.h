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
            VkDevice device = {};

            VkBuffer value = {};
            VkDeviceMemory memory = {};

            VkDeviceSize size = 0;

          public:
            BufferData() = delete;
            BufferData(const DeviceContext& devices, const BufferDataInfo& info);

            ~BufferData();
        };
    }
}