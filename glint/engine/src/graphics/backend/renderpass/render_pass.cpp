#include <array>
#include <stdexcept>

#include "glint/graphics/backend/renderpass/renderpass_data.h"
#include "glint/graphics/backend/swapchain/swapchain_data.h"

renderpass_data::renderpass_data(const swapchain_data& swapchain, VkRenderPassCreateInfo info, const VkImageView& depthImageView)
    : device(swapchain.device) {
    if (vkCreateRenderPass(device, &info, nullptr, &value) != VK_SUCCESS) {
        throw std::runtime_error("Vulkan | failed to create render pass!");
    }

    initFramebuffers(swapchain, depthImageView);
}

renderpass_data::~renderpass_data() {
    if (!framebuffers.empty()) {
        for (VkFramebuffer framebuffer : framebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
        framebuffers.clear();
    }

    if (value != VK_NULL_HANDLE) {
        vkDestroyRenderPass(device, value, nullptr);
        value = VK_NULL_HANDLE;
    }
}

void renderpass_data::initFramebuffers(const swapchain_data& swapchain, VkImageView depthImageView) {
    framebuffers.resize(swapchain.views.size());

    for (int i = 0; i < swapchain.views.size(); i++) {
        std::array<VkImageView, 2> attachments = {swapchain.views[i], depthImageView};

        VkFramebufferCreateInfo fbInfo = {};
        fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbInfo.renderPass = value;
        fbInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        fbInfo.pAttachments = attachments.data();
        fbInfo.width = swapchain.extent.width;
        fbInfo.height = swapchain.extent.height;
        fbInfo.layers = 1;

        if (vkCreateFramebuffer(device, &fbInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create framebuffer");
        }
    }
}