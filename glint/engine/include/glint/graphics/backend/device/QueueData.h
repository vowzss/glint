#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace glint::engine::graphics {

    struct QueueFamilySupportDetails;
    struct QueueFamiliesSupportDetails;

    struct QueueData {
      private:
        std::vector<VkQueue> handles;

      public:
        QueueData() = delete;
        QueueData(const VkDevice& device, const QueueFamilySupportDetails& family);

        ~QueueData() noexcept = default;

        // --- operators ---
        VkQueue& operator[](size_t i) noexcept {
            return handles[i];
        }
        const VkQueue& operator[](size_t i) const noexcept {
            return handles[i];
        }
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

        ~QueuesData() noexcept = default;
    };

}