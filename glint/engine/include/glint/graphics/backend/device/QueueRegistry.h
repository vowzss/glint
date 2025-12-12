#pragma once

#include "QueueObject.h"

namespace glint::engine::graphics {

    struct QueueFamiliesSupportDetails;

    struct QueueRegistry {
      public:
        QueueObject graphics; // for submitting drawing/rendering commands
        QueueObject present;  // for presenting rendered images to the window/surface
        QueueObject compute;  // for compute shader workloads
        QueueObject transfer; // for optimized buffer/image copy operations
        QueueObject sparse;   // for partially resident resources

      public:
        QueueRegistry() = delete;
        ~QueueRegistry() noexcept = default;

        QueueRegistry(const QueueRegistry&) = delete;
        QueueRegistry& operator=(const QueueRegistry&) = delete;

        QueueRegistry(QueueRegistry&&) noexcept = delete;
        QueueRegistry& operator=(QueueRegistry&&) noexcept = delete;

        QueueRegistry(const VkDevice& device, const QueueFamiliesSupportDetails& families);
    };

}