#include <stdexcept>

#include "glint/graphics/backend/VkHelpers.h"
#include "glint/graphics/backend/buffer/ImageBufferData.h"
#include "glint/graphics/backend/device/Devices.h"

namespace glint::engine::graphics {

    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t filter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties props;
        vkGetPhysicalDeviceMemoryProperties(device, &props);

        for (uint32_t i = 0; i < props.memoryTypeCount; i++) {
            if ((filter & (1 << i)) && (props.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("Vulkan | failed to find suitable memory type!");
    }

    VkFormat findSupportedImageFormat(const VkPhysicalDevice& device, const std::vector<VkFormat>& candidates, VkImageTiling tiling,
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

    VkFormat findDepthFormat(const VkPhysicalDevice& device) {
        return findSupportedImageFormat(device, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    ImageBufferData createDepthImage(const Devices& devices, VkExtent2D extent) {
        VkFormat depthImageFormat = findDepthFormat(devices.physical);
        ImageBufferDataInfo bufferInfo = {};
        bufferInfo.format = depthImageFormat;
        bufferInfo.extent = extent;

        return ImageBufferData{devices, bufferInfo};
    }

}