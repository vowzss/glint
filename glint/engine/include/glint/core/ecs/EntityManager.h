#pragma once

#include <cstdint>
#include <vector>

namespace glint::engine::core {

    struct EntityHandle {
        size_t id;
        uint32_t version;

      public:
        EntityHandle() = delete;
        EntityHandle(size_t id_, uint32_t version_) : id(id_), version(version_) {};

        bool operator==(const EntityHandle& other) const {
            return id == other.id && version == other.version;
        }
    };

    struct EntityMetadata {
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

        void incrementVersion() noexcept {
            uint32_t v = version() + 1;
            flags = (flags & AliveMask) | (v & VersionMask);
        }
    };

    struct EntityManager {
      private:
        std::vector<EntityMetadata> states;

        std::vector<uint32_t> freeIds;
        uint32_t nextId = 0;

      public:
        EntityManager() = default;

      public:
        EntityHandle create();

        void destroy(EntityHandle handle);

        inline bool isValid(EntityHandle handle) const {
            return handle.id < states.size() && states[handle.id].isAlive()
                   && states[handle.id].version() == handle.version;
        }
    };
}