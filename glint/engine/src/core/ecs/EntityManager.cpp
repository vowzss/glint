#include "glint/core/ecs/EntityManager.h"

namespace glint::engine::core {

    EntityHandle EntityManager::create() {
        uint32_t id = computeUniqueId();
        entries[id].setAlive(true);

        return EntityHandle{id, entries[id].version()};
    }

    void EntityManager::destroy(EntityHandle handle) {
        if (!handle.isValid() || handle.id >= entries.size()) return;

        auto& entry = entries[handle.id];
        if (!entry.isAlive()) return;

        entry.setAlive(false);
        entry.incrementVersion();

        freeIds.push_back(handle.id);
    }

    /*
            inline bool isValid(EntityHandle handle) const {
            return handle.id < entries.size() && entries[handle.id].isAlive() && entries[handle.id].version() == handle.version;
        }*/

    uint32_t EntityManager::computeUniqueId() {
        const uint32_t id = freeIds.empty() ? nextId++ : freeIds.back();

        if (!freeIds.empty()) {
            freeIds.pop_back();
        }

        if (id >= entries.size()) {
            entries.resize(id + 1);
        }

        return id;
    }

}