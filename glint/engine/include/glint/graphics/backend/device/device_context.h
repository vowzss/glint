#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {
    struct device_context {
        VkDevice logical;
        VkPhysicalDevice physical;

        device_context() = default;
    };
}