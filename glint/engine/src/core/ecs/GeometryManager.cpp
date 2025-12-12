#include "glint/core/ecs/GeometryManager.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/graphics/assets/Geometry.h"
#include "glint/graphics/backend/buffer/GeometryBufferObject.h"

using namespace glint::engine::graphics;

namespace glint::engine::core {

    GeometryHandle GeometryManager::create(const Devices& devices, const std::string& path) {
        AssetHandle<Geometry> data = m_assets.load<Geometry>(path);
        if (!data.valid()) return GeometryHandle::invalid();

        const Geometry* geometry = m_assets.get(data);
        if (geometry == nullptr) return GeometryHandle::invalid();

        const uint32_t id = m_freeIds.empty() ? m_nextId++ : m_freeIds.back();
        if (!m_freeIds.empty()) m_freeIds.pop_back();
        if (id >= m_entries.size()) m_entries.resize(id + 1);

        GeometryEntry& entry = m_entries[id];
        entry.value = std::make_unique<GeometryBufferObject>(devices, *geometry);

        return GeometryHandle{id, entry.version()};
    }

    void GeometryManager::destroy(GeometryHandle handle) {
        if (!isValid(handle)) return;

        GeometryEntry& entry = m_entries[handle.id()];
        entry.value.reset();
        entry.bump();

        m_freeIds.push_back(handle.id());
    }

    graphics::GeometryBufferObject* GeometryManager::get(GeometryHandle handle) noexcept {
        if (!isValid(handle)) return nullptr;
        return m_entries[handle.id()].value.get();
    }

    const graphics::GeometryBufferObject* GeometryManager::get(GeometryHandle handle) const noexcept {
        if (!isValid(handle)) return nullptr;
        return m_entries[handle.id()].value.get();
    }
}