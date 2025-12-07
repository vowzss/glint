#include "glint/core/ecs/EntityManager.h"

namespace glint::engine::core {

    EntityHandle EntityManager::create() {
        uint32_t id;

        if (freeIds.empty()) {
            id = nextId++;
            entries.push_back({});
        } else {
            id = freeIds.back();
            freeIds.pop_back();
        }

        entries[id].setAlive(true);
        return EntityHandle{id, entries[id].version()};
    }

    void EntityManager::destroy(EntityHandle handle) {
        if (handle.id >= entries.size()) return;

        auto& entry = entries[handle.id];
        if (!entry.isAlive()) return;

        entry.setAlive(false);
        entry.incrementVersion(); // increment to invalidate old handles

        freeIds.push_back(handle.id);
    }
}