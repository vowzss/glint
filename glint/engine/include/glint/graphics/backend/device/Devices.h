#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct Devices {
        VkDevice logical;
        VkPhysicalDevice physical;
    };

}