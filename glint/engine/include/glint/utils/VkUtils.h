#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine {
    namespace graphics {
        struct QueueFamiliesDetails;
        struct SwapchainDetails;
    }
}

namespace glint::engine::utils {

    inline std::vector<const char*> getDeviceExtensions() {
        static std::vector<const char*> entries = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
            // VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
            // VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME,
        };

#ifdef PLATFORM_MACOS
        // entries.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        entries.push_back("VK_KHR_portability_subset");
        // entries.push_back("VK_KHR_get_physical_device_properties2.");
#endif

        return entries;
    }

    inline const std::vector<const char*> getValidationLayers() {
        static std::vector<const char*> entries = {};

#ifdef BUILD_DEBUG
        entries.push_back("VK_LAYER_KHRONOS_validation");
#endif

        return entries;
    };

    struct VulkanVersion {
        uint32_t major;
        uint32_t minor;
        uint32_t patch;
    };

    VulkanVersion getVulkanLoaderVersion();
    VulkanVersion getDeviceVulkanVersion(VkPhysicalDevice device);

    VkPhysicalDevice selectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface, const std::vector<const char*>& extensions);
    VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR selectSurfacePresentMode(const std::vector<VkPresentModeKHR>& modes);
    uint32_t selectSurfaceImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
    VkExtent2D selectSurfaceExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities);

    graphics::QueueFamiliesDetails queryQueueFamiliesDetails(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
    graphics::SwapchainDetails querySwapchainDetails(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

    bool isDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, const std::vector<const char*>& extensions);
    bool isDeviceExtensionsSupported(const VkPhysicalDevice& device, const std::vector<const char*>& extensions);

    bool isValidationLayersSupported(const std::vector<const char*>& layers);
}
