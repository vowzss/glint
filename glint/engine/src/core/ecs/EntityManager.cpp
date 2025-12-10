#include "glint/core/ecs/EntityManager.h"

namespace glint::engine::core {

    EntityHandle EntityManager::create() {
        const uint32_t id = m_freeIds.empty() ? m_nextId++ : m_freeIds.back();
        if (!m_freeIds.empty()) m_freeIds.pop_back();
        if (id >= m_entries.size()) m_entries.resize(id + 1);

        EntityEntry& entry = m_entries[id];
        entry.setAlive(true);

        return EntityHandle{id, entry.version()};
    }

    void EntityManager::destroy(EntityHandle handle) {
        if (!handle.valid() || handle.id() >= m_entries.size()) return;

        EntityEntry& entry = m_entries[handle.id()];
        if (entry.version() != handle.version()) return;
        if (!entry.isAlive()) return;

        entry.setAlive(false);
        entry.bump();

        m_freeIds.push_back(handle.id());
    }

}