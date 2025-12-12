#pragma once

#include <atomic>

#include "glint/graphics/backend/CommandsPoolData.h"

namespace glint::engine::graphics {

    struct QueueFamilySupportDetails;

    class QueueObject {
        uint32_t m_family = UINT32_MAX;

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

        QueueObject(const VkDevice& device, const QueueFamilySupportDetails& family);

        // --- getters ---
        inline uint32_t count() const noexcept {
            return static_cast<uint32_t>(m_handles.size());
        }

        inline VkQueue handle() noexcept {
            size_t idx = m_nextHandle.fetch_add(1, std::memory_order_relaxed) % m_handles.size();
            return m_handles[idx];
        }

        inline VkQueue handle(size_t idx) noexcept {
            return m_handles[idx % m_handles.size()];
        }

        inline CommandsPoolObject& pool() noexcept {
            return m_pool;
        }

        inline const CommandsPoolObject& pool() const noexcept {
            return m_pool;
        }
    };

}