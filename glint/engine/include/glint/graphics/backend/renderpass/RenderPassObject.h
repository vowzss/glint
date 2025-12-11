#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct SwapchainObject;

    /*struct RenderpassCreateInfo {
        const SwapchainObject& swapchain,
        VkRenderPassCreateInfo info,
        const VkImageView& depthImageView,
    };*/

    struct RenderPassObject {
        const VkDevice device;

        VkRenderPass handle;
        std::vector<VkFramebuffer> framebuffers;

      public:
        RenderPassObject() = delete;
        RenderPassObject(const SwapchainObject& swapchain, VkRenderPassCreateInfo info, const VkImageView& depthImageView);

        ~RenderPassObject();
    };

}