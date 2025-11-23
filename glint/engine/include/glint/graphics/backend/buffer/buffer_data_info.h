#pragma once

#include "vulkan/vulkan_core.h"

namespace glint::engine::graphics::backend {
    struct buffer_data_info {
        const void* data;
        VkDeviceSize size;

        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };
}