#include "glint/utils/vk_utils.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

#include <sys/types.h>

using namespace glint;

VkPhysicalDevice utils::selectPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface) {
    // get number of GPUs available
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    if (count == 0) {
        throw std::runtime_error("Vulkan | no supported GPUs!");
    }

    // get list of physical devices
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(instance, &count, devices.data());

    // pick first suitable device
    for (const VkPhysicalDevice& device : devices) {
        if (utils::isDeviceSuitable(device, surface)) {
            return device;
        }
    }

    throw std::runtime_error("Vulkan | no suitable GPU!");
}

VkSurfaceFormatKHR utils::selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
    for (const VkSurfaceFormatKHR& f : formats) {
        if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return f;
        }
    }

    return formats[0];
}

VkPresentModeKHR utils::selectSurfacePresentMode(const std::vector<VkPresentModeKHR>& modes) {
    for (const VkPresentModeKHR& m : modes) {
        if (m == VK_PRESENT_MODE_MAILBOX_KHR) {
            return m;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t utils::selectSurfaceImageCount(const VkSurfaceCapabilitiesKHR& capabilities) {
    uint32_t count = capabilities.minImageCount + 1;

    // clamp to maximum if defined (0 means no limit)
    if (capabilities.maxImageCount > 0 && count > capabilities.maxImageCount) {
        count = capabilities.maxImageCount;
    }

    return count;
}

VkExtent2D utils::selectSurfaceExtent(int width, int height, const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    VkExtent2D extent;
    extent.width = std::clamp(width, (int)capabilities.minImageExtent.width, (int)capabilities.maxImageExtent.width);
    extent.height = std::clamp(height, (int)capabilities.minImageExtent.height, (int)capabilities.maxImageExtent.height);
    return extent;
}

queue_families_support_details utils::queryQueueFamiliesSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
    queue_families_support_details families;

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());

    // find supported queue families
    for (int i = 0; i < props.size(); i++) {
        const VkQueueFamilyProperties& prop = props[i];

        // can execute draw commands
        if ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !families.isGraphicsReady()) {
            families.graphics.index = i;
            families.graphics.count = 1;

            families.graphics.priorities.resize(1, 1.0f);
            /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                families.graphics.priorities[q] = 1.0f - 0.1f * q;
            }*/
        }

        // can execute compute shaders
        if ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) && !families.isComputeReady()) {
            families.compute.index = i;
            families.compute.count = 1;

            families.compute.priorities.resize(1, 1.0f);
            /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                families.compute.priorities[q] = 0.8f - 0.1f * q;
            }*/
        }

        // optimized for buffer/image copies
        if ((prop.queueFlags & VK_QUEUE_TRANSFER_BIT) && !families.isTransferReady()) {
            families.transfer.index = i;
            families.transfer.count = 1;

            families.transfer.priorities.resize(1, 1.0f);
            /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                families.transfer.priorities[q] = 0.3f - 0.05f * q;
            }*/
        }

        // supports partially resident resources
        if ((prop.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) && !families.isSparseReady()) {
            families.sparseBinding.index = i;
            families.sparseBinding.count = 1;

            families.sparseBinding.priorities.resize(1, 1.0f);
        }

        // can present images to the surface
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        if (present_support && !families.isPresentReady()) {
            families.present.index = i;
            families.present.count = 1;

            families.present.priorities.resize(1, 1.0f);
            /*for (uint32_t q = 0; q < prop.queueCount; q++) {
                families.present.priorities[q] = 0.9f - 0.05f * q;
            }*/
        }

        if (families.isReady()) {
            break;
        }
    }

    return families;
}

swapchain_support_details utils::querySwapchainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
    swapchain_support_details details;

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

bool utils::isDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
    // check required queues
    if (!utils::queryQueueFamiliesSupport(device, surface).isReady()) {
        return false;
    }

    // check required device extensions
    if (!isDeviceExtensionsSupported(device)) {
        return false;
    }

    // check swap chain support
    swapchain_support_details swapchainSupport = utils::querySwapchainSupport(device, surface);
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

bool utils::isDeviceExtensionsSupported(const VkPhysicalDevice& device) {
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> props(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, props.data());

    for (const char* ext : deviceExtensions) {
        bool found = false;

        for (const VkExtensionProperties& prop : props) {
            if (strcmp(ext, prop.extensionName) == 0) {
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

bool utils::isValidationLayersSupported() {
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