#pragma once

#include <memory>

#include <vulkan/vulkan_core.h>

#include "buffer/buffer_data.h"

namespace glint::engine::scene::components {
    struct camera;
}

namespace glint::engine::graphics {
    namespace backend {
        struct device_context;

        struct frame_data_info {
            const std::unique_ptr<glint::engine::scene::components::camera>& camera;
            VkDescriptorPool pool;
            VkDescriptorSetLayout layout;
        };

        struct frame_data {
            VkDevice device{};

            VkDescriptorSet cameraSet{};
            std::unique_ptr<buffer_data> cameraBuffer;

            VkSemaphore imageAvailable{};
            VkSemaphore renderFinished{};
            VkFence inFlight{};

          public:
            frame_data() = delete;
            frame_data(const device_context& devices, const frame_data_info& info);

            ~frame_data();
        };
    }
}