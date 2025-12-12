#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

#include "glint/graphics/backend/CommandsPoolData.h"

namespace glint::engine::graphics {

    struct QueueFamilyDetails;

    class QueueObject {
        std::vector<VkQueue> m_handles;
        std::atomic<size_t> m_nextHandle{0};

        CommandsPoolObject m_pool;

      public:
        QueueObject() = delete;
        ~QueueObject() noexcept = default;

        QueueObject(const QueueObject&) = delete;
        QueueObject& operator=(const QueueObject&) = delete;

        QueueObject(QueueObject&&) noexcept = delete;
        QueueObject& operator=(QueueObject&&) noexcept = delete;

        QueueObject(const VkDevice& device, const QueueFamilyDetails& family, uint32_t frames);

        // --- getters ---
        inline uint32_t count() const noexcept {
            return static_cast<uint32_t>(m_handles.size());
        }

        inline VkQueue handle() noexcept {
            size_t idx = m_nextHandle.fetch_add(1, std::memory_order_relaxed) % m_handles.size();
            return m_handles[idx];
        }

        inline CommandsPoolObject& pool() noexcept {
            return m_pool;
        }

        inline const CommandsPoolObject& pool() const noexcept {
            return m_pool;
        }

        // --- operators ---
        inline const VkQueue& operator[](size_t idx) const noexcept {
            return m_handles[idx % m_handles.size()];
        }
    };

}