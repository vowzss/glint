#pragma once

#include "vulkan/vulkan_core.h"

struct image_buffer_data_info {
    VkFormat format;
    VkExtent2D extent;
    VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
};