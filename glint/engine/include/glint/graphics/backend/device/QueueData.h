#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics::backend {

    struct QueueFamilySupportDetails;
    struct QueueFamiliesSupportDetails;

    struct QueueData {
      private:
        std::vector<VkQueue> values;

      public:
        QueueData() = delete;
        QueueData(const VkDevice& device, const QueueFamilySupportDetails& family);

        ~QueueData() = default;

        // --- operators ---
        VkQueue& operator[](size_t i) { return values[i]; }
        const VkQueue& operator[](size_t i) const { return values[i]; }
    };

    struct QueuesData {
      public:
        QueueData graphics; // for submitting drawing/rendering commands
        QueueData present;  // for presenting rendered images to the window/surface
        QueueData compute;  // for compute shader workloads
        QueueData transfer; // for optimized buffer/image copy operations
        QueueData sparse;   // for partially resident resources

      public:
        QueuesData() = delete;
        QueuesData(const VkDevice& device, const QueueFamiliesSupportDetails& families);

        ~QueuesData() = default;
    };

}