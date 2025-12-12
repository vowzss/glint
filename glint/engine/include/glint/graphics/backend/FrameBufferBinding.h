#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    template <typename T>
    struct FrameBufferBinding {
        VkDescriptorSet set;
        T buffer;
    };

}