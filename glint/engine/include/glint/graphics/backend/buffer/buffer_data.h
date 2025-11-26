#pragma once

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {
    struct device_context;
    struct buffer_data_info;

    struct buffer_data {
        VkDevice device{};

        VkBuffer value{};
        VkDeviceMemory memory{};

        VkDeviceSize size;

      public:
        buffer_data() = delete;
        buffer_data(const device_context& devices, const buffer_data_info& info);

        ~buffer_data();
    };
}