#pragma once

#include "vulkan/vulkan_core.h"

struct buffer_data_info {
    const void* data;
    VkDeviceSize size;

    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags properties;

    buffer_data_info() = delete;
};