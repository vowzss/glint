#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    namespace backend {
        struct DeviceContext;

        struct ImageBufferDataInfo {
            VkFormat format;
            VkExtent2D extent;
            VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

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

          public:
            ImageBufferData() = delete;
            ImageBufferData(const DeviceContext& devices, const ImageBufferDataInfo& info);

            ~ImageBufferData();
        };
    }
}