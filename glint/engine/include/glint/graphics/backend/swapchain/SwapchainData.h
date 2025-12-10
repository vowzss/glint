#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct SwapchainData {
        VkDevice device = {};

        VkSwapchainKHR value = {};
        const VkFormat format;
        const VkExtent2D extent;

        std::vector<VkImage> images = {};
        std::vector<VkImageView> views = {};

      public:
        SwapchainData() = delete;
        SwapchainData(VkDevice& device, VkSwapchainCreateInfoKHR info);

        ~SwapchainData();

      private:
        void initImages();
        void initViews();
    };

}