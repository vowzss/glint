#include "glint/core/ecs/EntityManager.h"

namespace glint::engine::core {

    EntityHandle EntityManager::create() {
        const uint32_t id = m_freeIds.empty() ? m_nextId++ : m_freeIds.back();
        if (!m_freeIds.empty()) m_freeIds.pop_back();
        if (id >= m_entries.size()) m_entries.resize(id + 1);

        EntityEntry& entry = m_entries[id];
        entry.setAlive(true);
        m_aliveIds.push_back(id);

        return EntityHandle{id, entry.version()};
    }

    void EntityManager::destroy(EntityHandle handle) {
        if (!isValid(handle)) return;

        EntityEntry& entry = m_entries[handle.id()];
        entry.setAlive(false);
        entry.bump();

        m_freeIds.push_back(handle.id());

        auto it = std::find(m_aliveIds.begin(), m_aliveIds.end(), handle.id());
        if (it != m_aliveIds.end()) {
            *it = m_aliveIds.back();
            m_aliveIds.pop_back();
        }
    }

    std::vector<EntityHandle> EntityManager::query() const {
        std::vector<EntityHandle> handles;
        handles.reserve(m_aliveIds.size());

        for (uint32_t id : m_aliveIds) {
            handles.push_back(EntityHandle{id, m_entries[id].version()});
        }

        return handles;
    }

}