#pragma once

#include <vector>

#include "glint/graphics/backend/device/queue_family_support_details.h"
#include "glint/graphics/backend/swapchain/swapchain_support_details.h"

// todo: cleanup this bullshit
using namespace glint::engine::graphics::backend;

namespace glint::engine::utils {
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        // VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
        // VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME,
    };

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