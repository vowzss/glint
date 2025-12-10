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
        bool isAlive() const noexcept {
            return (flags & AliveMask) != 0;
        }

        uint32_t version() const noexcept {
            return flags & VersionMask;
        }

        void setAlive(bool alive) noexcept {
            if (alive) {
                flags |= AliveMask;
            } else {
                flags &= ~AliveMask;
            }
        }

        inline void bump() noexcept {
            uint32_t current = version();
            uint32_t next = current + 1;

            if (next > VersionMask) {
                std::cerr << "[Warning] GeometryEntry version exceeded max value (" << VersionMask << ") and will wrap around.\n";
                next = 0;
            }

            flags = (flags & AliveMask) | (next & VersionMask);
        }
    };

    class EntityManager {
        std::vector<EntityEntry> m_entries;

        std::vector<uint32_t> m_freeIds;
        uint32_t m_nextId = 0;

      public:
        EntityManager() = default;
        ~EntityManager() = default;

        EntityHandle create();
        void destroy(EntityHandle handle);

        inline bool isAlive(EntityHandle handle) const noexcept {
            return handle.id() < m_entries.size() && m_entries[handle.id()].isAlive();
        }
    };

}