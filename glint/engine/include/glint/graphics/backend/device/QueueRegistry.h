#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

#include "QueueObject.h"

namespace glint::engine::graphics {

    struct QueueFamiliesDetails;

    struct QueueRegistry {
      public:
        QueueObject graphics;
        QueueObject present;
        QueueObject compute;
        std::optional<QueueObject> transfer = std::nullopt;
        std::optional<QueueObject> sparse = std::nullopt;

      public:
        QueueRegistry() = delete;
        ~QueueRegistry() noexcept = default;

        QueueRegistry(const QueueRegistry&) = delete;
        QueueRegistry& operator=(const QueueRegistry&) = delete;

        QueueRegistry(QueueRegistry&&) noexcept = delete;
        QueueRegistry& operator=(QueueRegistry&&) noexcept = delete;

        QueueRegistry(const VkDevice& device, const QueueFamiliesDetails& families, uint32_t frames);
    };

}