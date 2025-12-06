#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct DeviceContext {
        VkDevice logical;
        VkPhysicalDevice physical;

      public:
        DeviceContext() = default;
    };

}