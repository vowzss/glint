#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

#include "glint/models/misc/queue_family_support_details.h"
#include "glint/models/misc/swapchain_support_details.h"

namespace glint::utils {
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    VkPhysicalDevice selectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);

    VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR selectSurfacePresentMode(const std::vector<VkPresentModeKHR>& modes);
    uint32_t selectSurfaceImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
    VkExtent2D selectSurfaceExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities);

    queue_families_support_details queryQueueFamiliesSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
    swapchain_support_details querySwapchainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

    bool isDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
    bool isDeviceExtensionsSupported(const VkPhysicalDevice& device);

    bool isValidationLayersSupported();
}