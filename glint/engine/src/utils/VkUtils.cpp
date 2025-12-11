#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "glint/graphics/backend/device/QueueFamilySupportDetails.h"
#include "glint/graphics/backend/swapchain/SwapchainSupportDetails.h"
#include "glint/utils/VkUtils.h"

using namespace glint::engine::graphics;

namespace glint::engine::utils {

    VulkanVersion getVulkanLoaderVersion() {
        VulkanVersion version{0, 0, 0};

        uint32_t apiVersion = 0;
        VkResult result = vkEnumerateInstanceVersion(&apiVersion);

        if (result != VK_SUCCESS) {
            version.major = 1;
            version.minor = 0;
            version.patch = 0;
        } else {
            version.major = VK_VERSION_MAJOR(apiVersion);
            version.minor = VK_VERSION_MINOR(apiVersion);
            version.patch = VK_VERSION_PATCH(apiVersion);
        }

        return version;
    }

    VulkanVersion getDeviceVulkanVersion(VkPhysicalDevice device) {
        VulkanVersion version{0, 0, 0};

        if (device == VK_NULL_HANDLE) {
            std::cerr << "Invalid physical device!\n";
            return version;
        }

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        version.major = VK_VERSION_MAJOR(properties.apiVersion);
        version.minor = VK_VERSION_MINOR(properties.apiVersion);
        version.patch = VK_VERSION_PATCH(properties.apiVersion);

        return version;
    }

    VkPhysicalDevice selectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface) {
        // get number of GPUs available
        uint32_t count = 0;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        if (count == 0) {
            throw std::runtime_error("Vulkan | No supported GPUs!");
        }

        // get list of physical devices
        std::vector<VkPhysicalDevice> devices(count);
        vkEnumeratePhysicalDevices(instance, &count, devices.data());

        // pick first suitable device
        for (const VkPhysicalDevice& device : devices) {
            if (isDeviceSuitable(device, surface)) {
                return device;
            }
        }

        throw std::runtime_error("Vulkan | No suitable GPU!");
    }

    VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
        for (const VkSurfaceFormatKHR& f : formats) {
            if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return f;
            }
        }

        return formats[0];
    }

    VkPresentModeKHR selectSurfacePresentMode(const std::vector<VkPresentModeKHR>& modes) {
        for (const VkPresentModeKHR& m : modes) {
            if (m == VK_PRESENT_MODE_MAILBOX_KHR) {
                return m;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    uint32_t selectSurfaceImageCount(const VkSurfaceCapabilitiesKHR& capabilities) {
        uint32_t count = capabilities.minImageCount;

        // clamp to maximum if defined (0 means no limit)
        if (capabilities.maxImageCount > 0 && count > capabilities.maxImageCount) {
            count = capabilities.maxImageCount;
        }

        return count;
    }

    VkExtent2D selectSurfaceExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }

        VkExtent2D extent;
        extent.width = std::clamp(width, (int)capabilities.minImageExtent.width, (int)capabilities.maxImageExtent.width);
        extent.height = std::clamp(height, (int)capabilities.minImageExtent.height, (int)capabilities.maxImageExtent.height);
        return extent;
    }

    QueueFamiliesSupportDetails queryQueueFamiliesSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
        QueueFamiliesSupportDetails families;

        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

        std::vector<VkQueueFamilyProperties> props(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());

        // find supported queue families
        for (size_t i = 0; i < props.size(); i++) {
            const VkQueueFamilyProperties& prop = props[i];

            // can execute draw commands
            if ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !families.graphicsAvailable()) {
                families.graphics.index = i;
                families.graphics.count = 1;

                families.graphics.priorities.resize(1, 1.0f);
                /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                    families.graphics.priorities[q] = 1.0f - 0.1f * q;
                }*/
            }

            // can execute compute shaders
            if ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) && !families.computeAvailable()) {
                families.compute.index = i;
                families.compute.count = 1;

                families.compute.priorities.resize(1, 1.0f);
                /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                    families.compute.priorities[q] = 0.8f - 0.1f * q;
                }*/
            }

            // optimized for buffer/image copies
            if ((prop.queueFlags & VK_QUEUE_TRANSFER_BIT) && !families.transferAvailable()) {
                families.transfer.index = i;
                families.transfer.count = 1;

                families.transfer.priorities.resize(1, 1.0f);
                /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                    families.transfer.priorities[q] = 0.3f - 0.05f * q;
                }*/
            }

            // supports partially resident resources
            if ((prop.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) && !families.sparseAvailable()) {
                families.sparseBinding.index = i;
                families.sparseBinding.count = 1;

                families.sparseBinding.priorities.resize(1, 1.0f);
            }

            // can present images to the surface
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
            if (present_support && !families.presentAvailable()) {
                families.present.index = i;
                families.present.count = 1;

                families.present.priorities.resize(1, 1.0f);
                /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                    families.present.priorities[q] = 0.9f - 0.05f * q;
                }*/
            }

            if (families.available()) {
                break;
            }
        }

        return families;
    }

    SwapchainSupportDetails querySwapchainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
        SwapchainSupportDetails details;

        // surface capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        // supported surface formats
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        // supported present modes
        uint32_t modeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);
        if (modeCount != 0) {
            details.modes.resize(modeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, details.modes.data());
        }

        return details;
    }

    bool isDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
        // check required queues
        if (!queryQueueFamiliesSupport(device, surface).available()) {
            return false;
        }

        // check required device extensions
        if (!isDeviceExtensionsSupported(device)) {
            return false;
        }

        // check swap chain support
        SwapchainSupportDetails swapchainSupport = querySwapchainSupport(device, surface);
        if (swapchainSupport.formats.empty() || swapchainSupport.modes.empty()) {
            return false;
        }

        // todo: check required device features
        // https://vkdoc.net/man/VkPhysicalDeviceVulkan13Features#man-header
        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        /*if (!supportedFeatures.samplerAnisotropy) {
            return false;
        }*/

        return true;
    }

    bool isDeviceExtensionsSupported(const VkPhysicalDevice& device) {
        uint32_t count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
        if (count == 0) return false;

        std::vector<VkExtensionProperties> props(count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, props.data());

        for (const char* ext : deviceExtensions) {
            bool found = false;
            for (const auto& prop : props) {
                if (std::string_view(ext) == prop.extensionName) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }

        return true;
    }

    bool isValidationLayersSupported() {
        uint32_t count;
        vkEnumerateInstanceLayerProperties(&count, nullptr);

        std::vector<VkLayerProperties> props(count);
        vkEnumerateInstanceLayerProperties(&count, props.data());

        for (const char* layer : validationLayers) {
            bool found = false;

            for (const VkLayerProperties& prop : props) {
                if (strcmp(layer, prop.layerName) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                return false;
            }
        }

        return true;
    }

}