#pragma once

#include <vulkan/vulkan_core.h>

struct device_context {
    VkDevice logical;
    VkPhysicalDevice physical;

    device_context() = default;
};