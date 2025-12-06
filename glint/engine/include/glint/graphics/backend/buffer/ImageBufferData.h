#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct DeviceContext;

    struct ImageBufferDataInfo {
        VkFormat format;
        VkExtent2D extent;
        VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        VkMemoryPropertyFlagBits properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

      public:
        ImageBufferDataInfo() = default;
    };

    struct ImageBufferData {
        VkDevice device = {};

        VkImage image = {};
        VkDeviceMemory memory = {};
        VkImageView view = {};

        VkFormat format;
        VkExtent2D extent;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlagBits properties;

      public:
        ImageBufferData() = delete;
        ImageBufferData(const DeviceContext& devices, const ImageBufferDataInfo& info);

        ~ImageBufferData();
    };

}