#include "glint/misc/vk_helpers.h"

#include <stdexcept>

#include "glint/graphics/backend/buffer/image_buffer_data.h"
#include "glint/graphics/backend/buffer/image_buffer_data_info.h"
#include "glint/graphics/backend/device/device_context.h"

using namespace glint;

uint32_t helpers::findMemoryType(VkPhysicalDevice device, uint32_t filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties props;
    vkGetPhysicalDeviceMemoryProperties(device, &props);

    for (uint32_t i = 0; i < props.memoryTypeCount; i++) {
        if ((filter & (1 << i)) && (props.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Vulkan | failed to find suitable memory type!");
}

image_buffer_data helpers::createDepthImage(const device_context& devices, VkExtent2D extent) {
    VkFormat depthImageFormat = findDepthFormat(devices.physical);
    image_buffer_data_info info = {};
    info.format = depthImageFormat;
    info.extent = extent;

    return image_buffer_data{devices, info};
}

VkFormat helpers::findSupportedImageFormat(const VkPhysicalDevice& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling,
    VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(device, format, &props);

        bool supported = false;
        if (tiling == VK_IMAGE_TILING_LINEAR) {
            supported = (props.linearTilingFeatures & features) == features;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL) {
            supported = (props.optimalTilingFeatures & features) == features;
        }

        if (supported) {
            return format;
        }
    }

    throw std::runtime_error("Vulkan | failed to find a supported image format!");
}

VkFormat helpers::findDepthFormat(const VkPhysicalDevice& device) {
    return helpers::findSupportedImageFormat(device, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}