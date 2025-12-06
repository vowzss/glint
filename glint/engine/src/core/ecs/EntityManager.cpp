#include "glint/core/ecs/EntityManager.h"

namespace glint::engine::core {

    EntityHandle EntityManager::create() {
        uint32_t id;

        if (freeIds.empty()) {
            id = nextId++;
            states.push_back({});
        } else {
            id = freeIds.back();
            freeIds.pop_back();
        }

        states[id].setAlive(true);
        return EntityHandle{id, states[id].version()};
    }

    void EntityManager::destroy(EntityHandle handle) {
        if (handle.id >= states.size()) return;

        auto& state = states[handle.id];
        if (!state.isAlive()) return;

        state.setAlive(false);
        state.incrementVersion(); // increment to invalidate old handles

        freeIds.push_back(handle.id);
    }

}