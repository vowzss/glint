#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {
    namespace backend {
        struct ImageBufferData;
        struct DeviceContext;

        uint32_t findMemoryType(VkPhysicalDevice device, uint32_t filter, VkMemoryPropertyFlags properties);

        VkFormat findSupportedImageFormat(const VkPhysicalDevice& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling,
            VkFormatFeatureFlags features);

        VkFormat findDepthFormat(const VkPhysicalDevice& device);

        ImageBufferData createDepthImage(const DeviceContext& devices, VkExtent2D extent);
    }
}