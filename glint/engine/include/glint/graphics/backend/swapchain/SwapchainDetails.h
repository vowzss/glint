#pragma once
#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct SwapchainDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> modes;

      public:
        SwapchainDetails() = default;
    };

}