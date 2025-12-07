#pragma once
#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats = {};
        std::vector<VkPresentModeKHR> modes = {};

      public:
        SwapchainSupportDetails() = default;
    };

}