#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "ObjectHandle.h"

namespace glint::engine::core {

    struct EntityEntry {
      private:
        static constexpr uint32_t AliveMask = 0x80000000;
        static constexpr uint32_t VersionMask = 0x7FFFFFFF;

      public:
        uint32_t flags = 0;

      public:
        uint32_t version() const noexcept {
            return flags & VersionMask;
        }

        inline void bump() noexcept {
            flags = (flags & AliveMask) | ((version() + 1) & VersionMask);
        }

        bool isAlive() const noexcept {
            return (flags & AliveMask) != 0;
        }

        void setAlive(bool alive) noexcept {
            flags = (flags & ~AliveMask) | (static_cast<uint32_t>(alive) << 31);
        }
    };

    class EntityManager {
        std::vector<EntityEntry> m_entries;
        std::vector<uint32_t> m_aliveIds;

        std::vector<uint32_t> m_freeIds;
        uint32_t m_nextId = 0;

      public:
        EntityManager() noexcept = default;
        ~EntityManager() noexcept = default;

        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;

        EntityManager(EntityManager&&) = delete;
        EntityManager& operator=(EntityManager&&) = delete;

        // --- methods ---
        EntityHandle create();
        void destroy(EntityHandle handle);

        inline bool isValid(EntityHandle handle) const noexcept {
            // clang-format off
            return handle.id() < m_entries.size() 
                && m_entries[handle.id()].isAlive() 
                && m_entries[handle.id()].version() == handle.version();
            // clang-format on
        }

        std::vector<EntityHandle> query() const;
    };

}