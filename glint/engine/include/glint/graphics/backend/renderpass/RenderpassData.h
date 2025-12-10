#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct SwapchainData;

    /*struct RenderpassCreateInfo {
        const SwapchainData& swapchain,
        VkRenderPassCreateInfo info,
        const VkImageView& depthImageView,
    };*/

    struct RenderpassData {
        const VkDevice device;

        VkRenderPass value;
        std::vector<VkFramebuffer> framebuffers;

      public:
        RenderpassData() = delete;
        RenderpassData(const SwapchainData& swapchain, VkRenderPassCreateInfo info, const VkImageView& depthImageView);

        ~RenderpassData();

      private:
        void initFramebuffers(const SwapchainData& swapchain, VkImageView depthImageView);
    };

}