#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

struct swapchain_data;

struct renderpass_data {
    VkDevice device = VK_NULL_HANDLE;

    VkRenderPass value = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> framebuffers;

  public:
    renderpass_data() = delete;
    renderpass_data(const swapchain_data& swapchain, VkRenderPassCreateInfo info, const VkImageView& depthImageView);

    ~renderpass_data();

  private:
    void initFramebuffers(const swapchain_data& swapchain, VkImageView depthImageView);
};