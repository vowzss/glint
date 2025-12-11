#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct Devices;

    struct ImageBufferInfo {
        VkFormat format;
        VkExtent2D extent;

        VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        VkMemoryPropertyFlagBits properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    };

    struct ImageBufferObject {
        VkDevice device;

        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;

        VkFormat format;
        VkExtent2D extent;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlagBits properties;

      public:
        ImageBufferObject() = delete;
        ImageBufferObject(const Devices& devices, const ImageBufferInfo& info);

        ~ImageBufferObject();
    };

}