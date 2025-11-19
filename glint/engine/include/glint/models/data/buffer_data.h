#pragma once

#include <vulkan/vulkan_core.h>

struct device_context;
struct buffer_data_info;

struct buffer_data {
    VkDevice device = VK_NULL_HANDLE;

    VkBuffer value = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDeviceSize size;

  public:
    buffer_data() = delete;
    buffer_data(const device_context& devices, const buffer_data_info& info);

    ~buffer_data();
};
