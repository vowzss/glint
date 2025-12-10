#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct RenderpassAttachmentInfo {
        VkAttachmentReference m_reference;
        VkAttachmentDescription m_description;

      protected:
        RenderpassAttachmentInfo() = default;
    };

    struct ColorAttachmentInfo : RenderpassAttachmentInfo {
        ColorAttachmentInfo(VkFormat format, uint32_t idx) {
            m_reference.attachment = idx;
            m_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            m_description = {};
            m_description.format = format;
            m_description.samples = VK_SAMPLE_COUNT_1_BIT;
            m_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            m_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            m_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            m_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            m_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            m_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }
    };

    struct DepthAttachmentInfo : RenderpassAttachmentInfo {
        DepthAttachmentInfo(VkFormat format, uint32_t idx) {
            m_reference.attachment = idx;
            m_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            m_description = {};
            m_description.format = format;
            m_description.samples = VK_SAMPLE_COUNT_1_BIT;
            m_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            m_description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            m_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            m_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            m_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            m_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }
    };

}