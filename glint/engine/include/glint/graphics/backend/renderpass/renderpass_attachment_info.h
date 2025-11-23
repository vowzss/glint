#pragma once

#include "vulkan/vulkan_core.h"

namespace glint::engine::graphics::backend {
    struct renderpass_attachment_info {
        VkAttachmentDescription description{};
        VkAttachmentReference reference{};

        renderpass_attachment_info() = default;
    };

    struct color_attachment_info : renderpass_attachment_info {
        color_attachment_info(VkFormat format, uint32_t idx) {
            description.format = format;
            description.samples = VK_SAMPLE_COUNT_1_BIT;
            description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            reference.attachment = idx;
            reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
    };

    struct depth_attachment_info : renderpass_attachment_info {
        depth_attachment_info(VkFormat format, uint32_t idx) {
            description.format = format;
            description.samples = VK_SAMPLE_COUNT_1_BIT;
            description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            reference.attachment = idx;
            reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }
    };
}