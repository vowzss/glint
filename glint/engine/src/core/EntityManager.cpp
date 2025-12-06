#include "glint/core/EntityManager.h"

namespace glint::engine::core {
    EntityHandle EntityManager::create() {
        uint32_t id;

        if (!pool.empty()) {
            id = pool.back();
            pool.pop_back();
            metadata[id].alive = true;
        } else {
            id = next++;
            metadata.push_back({true, 0});
        }

        return EntityHandle{id};
    }

    void EntityManager::destroy(EntityHandle handle) {
        auto& meta = metadata[handle.id];
        if (!meta.alive) return;

        meta.alive = false;
        meta.version++; // increment to invalidate old handles

        pool.push_back(handle.id);
    }
}