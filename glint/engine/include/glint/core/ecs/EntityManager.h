#pragma once

#include <cstdint>
#include <vector>

namespace glint::engine::core {
    struct EntityMetadata {
        bool alive = false;
        uint32_t version = 0;
    };

    struct EntityHandle {
        uint32_t id;

      public:
        EntityHandle() = delete;
        EntityHandle(uint32_t id_) : id(id_){};

        bool operator==(const EntityHandle& other) const { return id == other.id; }
    };

    struct EntityManager {
      private:
        std::vector<EntityMetadata> metadata;
        std::vector<uint32_t> pool = {};
        uint32_t next = 0;

      public:
        EntityHandle create();
        void destroy(EntityHandle handle);
    };
}