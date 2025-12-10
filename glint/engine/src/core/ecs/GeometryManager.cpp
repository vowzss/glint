#include "glint/core/ecs/GeometryManager.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/graphics/backend/buffer/GeometryBuffer.h"
#include "glint/graphics/models/GeometryData.h"

using namespace glint::engine::graphics;

namespace glint::engine::core {

    GeometryHandle GeometryManager::create(const Devices& devices, const std::string& path) {
        AssetHandle<GeometryData> data = m_assets->load<GeometryData>(path);
        if (!data.isValid()) return GeometryHandle::invalid();

        const GeometryData* geometry = m_assets->get(data);
        if (geometry == nullptr) return GeometryHandle::invalid();

        const uint32_t id = m_freeIds.empty() ? m_nextId++ : m_freeIds.back();
        if (!m_freeIds.empty()) m_freeIds.pop_back();
        if (id >= m_entries.size()) m_entries.resize(id + 1);

        GeometryEntry& entry = m_entries[id];
        entry.buffer.emplace(devices, *geometry);

        return GeometryHandle{id, entry.version()};
    }

    void GeometryManager::destroy(GeometryHandle handle) {
        if (!isValid(handle)) return;

        GeometryEntry& entry = m_entries[handle.id()];
        entry.buffer.reset();
        entry.bump();

        m_freeIds.push_back(handle.id());
    }

    graphics::GeometryBuffer* GeometryManager::get(GeometryHandle handle) noexcept {
        if (!isValid(handle)) return nullptr;
        return &m_entries[handle.id()].buffer.value();
    }

    const graphics::GeometryBuffer* GeometryManager::get(GeometryHandle handle) const noexcept {
        if (!isValid(handle)) return nullptr;
        return &m_entries[handle.id()].buffer.value();
    }
}