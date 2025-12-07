#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct DeviceHandles {
        VkDevice logical;
        VkPhysicalDevice physical;

      public:
        DeviceHandles() = default;
    };

}