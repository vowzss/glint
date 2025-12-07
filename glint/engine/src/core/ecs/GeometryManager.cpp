#include "glint/core/AssetManager.h"
#include "glint/core/ecs/GeometryManager.h"
#include "glint/graphics/backend/buffer/GeometryBuffer.h"
#include "glint/graphics/models/GeometryData.h"

namespace glint::engine::core {
    using namespace graphics::models;
    using namespace graphics::backend;

    GeometryHandle GeometryManager::create(const Devices& devices, const std::string& path) {
        AssetHandle<GeometryData> asset = assets->load<GeometryData>(path);
        if (!asset.isValid()) return GeometryHandle::invalid();

        const GeometryData* geometry = assets->get(asset);
        if (geometry == nullptr) return GeometryHandle::invalid();

        uint32_t id = computeUniqueId();
        entries[id] = std::make_shared<GeometryBuffer>(devices, *geometry);

        return GeometryHandle{id, 0};
    }

    void GeometryManager::destroy(GeometryHandle handle) {
        if (!handle.isValid() || handle.id >= entries.size()) return;

        entries[handle.id].reset();
        handle.version++;

        freeIds.push_back(handle.id);
    }

    uint32_t GeometryManager::computeUniqueId() {
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