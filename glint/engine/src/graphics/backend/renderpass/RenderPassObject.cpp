#include <array>
#include <cstddef>
#include <stdexcept>

#include "glint/graphics/backend/renderpass/RenderPassObject.h"
#include "glint/graphics/backend/swapchain/SwapchainObject.h"

namespace glint::engine::graphics {

    RenderPassObject::RenderPassObject(const SwapchainObject& swapchain, VkRenderPassCreateInfo info, const VkImageView& depthImageView)
        : device(swapchain.device) {
        if (vkCreateRenderPass(device, &info, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | Failed to create render pass!");
        }

        framebuffers.resize(swapchain.views.size());

        for (size_t i = 0; i < swapchain.views.size(); i++) {
            std::array<VkImageView, 2> attachments = {swapchain.views[i], depthImageView};

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = handle;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapchain.extent.width;
            framebufferInfo.height = swapchain.extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | Failed to create framebuffer");
            }
        }
    }

    RenderPassObject::~RenderPassObject() {
        if (!framebuffers.empty()) {
            for (VkFramebuffer framebuffer : framebuffers) {
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            }
            framebuffers.clear();
        }

        if (handle != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device, handle, nullptr);
            handle = VK_NULL_HANDLE;
        }
    }

}