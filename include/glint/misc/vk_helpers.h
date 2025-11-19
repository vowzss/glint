#pragma once

#include <cstdint>
#include <vector>

#include "glint/models/data/image_buffer_data.h"

namespace glint::helpers {
    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t filter, VkMemoryPropertyFlags properties);

    VkFormat findSupportedImageFormat(const VkPhysicalDevice& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat(const VkPhysicalDevice& device);

    image_buffer_data createDepthImage(const device_context& devices, VkExtent2D extent);
}