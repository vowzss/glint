#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {
    namespace backend {
        struct DeviceContext {
            VkDevice logical;
            VkPhysicalDevice physical;

          public:
            DeviceContext() = default;
        };
    }
}