#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct SwapchainObject {
        VkDevice device;

        VkSwapchainKHR handle;
        const VkFormat format;
        const VkExtent2D extent;

        std::vector<VkImage> images;
        std::vector<VkImageView> views;

      public:
        SwapchainObject() = delete;
        SwapchainObject(VkDevice& device, VkSwapchainCreateInfoKHR info);

        ~SwapchainObject();

      private:
        void initImages();
        void initViews();
    };

}