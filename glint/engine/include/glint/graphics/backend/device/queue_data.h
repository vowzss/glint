#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {
    struct queue_family_support_details;
    struct queue_families_support_details;

    struct queue_data {
      private:
        std::vector<VkQueue> values;

      public:
        queue_data() = delete;
        queue_data(const VkDevice& device, const queue_family_support_details& family);

        ~queue_data() = default;

        // --- operators ---
        VkQueue& operator[](size_t i) { return values[i]; }
        const VkQueue& operator[](size_t i) const { return values[i]; }
    };

    struct queues_data {
        queue_data graphics; // for submitting drawing/rendering commands
        queue_data present;  // for presenting rendered images to the window/surface
        queue_data compute;  // for compute shader workloads
        queue_data transfer; // for optimized buffer/image copy operations
        queue_data sparse;   // for partially resident resources

        queues_data() = delete;
        queues_data(const VkDevice& device, const queue_families_support_details& families);

        ~queues_data() = default;
    };
}