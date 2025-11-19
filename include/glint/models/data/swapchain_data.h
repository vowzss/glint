#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

struct swapchain_data {
    VkDevice device = VK_NULL_HANDLE;

    VkSwapchainKHR value = VK_NULL_HANDLE;
    const VkFormat format;
    const VkExtent2D extent;

    std::vector<VkImage> images{};
    std::vector<VkImageView> views{};

  public:
    swapchain_data() = delete;
    swapchain_data(VkDevice& device, VkSwapchainCreateInfoKHR info);

    ~swapchain_data();

  private:
    void initImages();
    void initViews();
};