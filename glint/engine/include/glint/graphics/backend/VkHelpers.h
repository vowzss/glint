#pragma once

#include <cstdint>
#include <vector>

#include "glint/graphics/backend/device/Devices.h"

namespace glint::engine::graphics {

    struct ImageBufferObject;

    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t filter, VkMemoryPropertyFlags properties);

    VkFormat findSupportedImageFormat(VkPhysicalDevice device, std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features);

    VkFormat findDepthFormat(VkPhysicalDevice device);

    ImageBufferObject createDepthImage(const Devices& devices, VkExtent2D extent);

}