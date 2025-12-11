#include <stdexcept>

#include "glint/graphics/backend/swapchain/SwapchainObject.h"

namespace glint::engine::graphics {

    SwapchainObject::SwapchainObject(VkDevice& dev, VkSwapchainCreateInfoKHR info) : device(dev), format(info.imageFormat), extent(info.imageExtent) {
        if (vkCreateSwapchainKHR(device, &info, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create swapchain!");
        }

        initImages();
        initViews();
    }

    SwapchainObject::~SwapchainObject() {
        if (!views.empty()) {
            for (VkImageView view : views) {
                vkDestroyImageView(device, view, nullptr);
            }
            views.clear();
        }

        if (handle != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(device, handle, nullptr);
            handle = VK_NULL_HANDLE;
        }

        if (!images.empty()) {
            images.clear();
        }
    }

    void SwapchainObject::initImages() {
        // query the number of images in the swapchain
        uint32_t count;
        vkGetSwapchainImagesKHR(device, handle, &count, nullptr);

        if (count == 0) {
            return;
        }

        if (images.size() != count) {
            images.resize(count);
        }

        // retrieve the image handles
        vkGetSwapchainImagesKHR(device, handle, &count, images.data());
    }

    void SwapchainObject::initViews() {
        views.resize(images.size());

        for (size_t i = 0; i < images.size(); i++) {
            VkImageViewCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            info.image = images[i];
            info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            info.format = format;
            info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.baseMipLevel = 0;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.baseArrayLayer = 0;
            info.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &info, nullptr, &views[i]) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan | failed to create swapchain image view!");
            }
        }
    }

}