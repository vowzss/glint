#include <array>
#include <stdexcept>

#include "glint/graphics/backend/renderpass/RenderpassData.h"
#include "glint/graphics/backend/swapchain/SwapchainData.h"

namespace glint::engine::graphics {
    namespace backend {
        RenderpassData::RenderpassData(const SwapchainData& swapchain, VkRenderPassCreateInfo info, const VkImageView& depthImageView)
            : device(swapchain.device) {
            if (vkCreateRenderPass(device, &info, nullptr, &value) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create render pass!");
            }

            initFramebuffers(swapchain, depthImageView);
        }

        RenderpassData::~RenderpassData() {
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

        void RenderpassData::initFramebuffers(const SwapchainData& swapchain, VkImageView depthImageView) {
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
    }
}