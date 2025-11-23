#pragma once

#include <vulkan/vulkan_core.h>

struct device_context;
struct image_buffer_data_info;

struct image_buffer_data {
    VkDevice device = VK_NULL_HANDLE;

    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;

    VkFormat format;
    VkExtent2D extent;
    VkImageUsageFlags usage;

    image_buffer_data() = delete;
    image_buffer_data(const device_context& devices, const image_buffer_data_info& info);

    ~image_buffer_data();
};