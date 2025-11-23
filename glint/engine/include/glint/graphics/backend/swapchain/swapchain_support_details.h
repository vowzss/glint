#pragma once
#include <vector>

#include "vulkan/vulkan_core.h"

namespace glint::engine::graphics::backend {
    struct swapchain_support_details {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> modes;

      public:
        swapchain_support_details() = default;
    };
}