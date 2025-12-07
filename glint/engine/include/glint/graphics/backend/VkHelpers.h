#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct ImageBufferData;
    struct DeviceHandles;

    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t filter, VkMemoryPropertyFlags properties);

    VkFormat findSupportedImageFormat(const VkPhysicalDevice& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features);

    VkFormat findDepthFormat(const VkPhysicalDevice& device);

    ImageBufferData createDepthImage(const DeviceHandles& devices, VkExtent2D extent);

}