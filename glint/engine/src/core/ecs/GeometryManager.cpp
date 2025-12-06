#include "glint/core/Logger.h"
#include "glint/core/ecs/GeometryManager.h"
#include "glint/loaders/GeometryLoader.h"
#include "glint/scene/components/GeometryComponent.h"

namespace glint::engine::core {
    using namespace scene::components;
    using namespace loaders;

    GeometryManager::GeometryManager() {
        loader = std::make_unique<GeometryLoader>();
    }

    GeometryManager::~GeometryManager() {
        entries.clear();
        loader.reset();
    }

    std::optional<GeometryCreateInfo> GeometryManager::load(const std::string& filename) {
        auto meshInfo = loader->load(filename);

        if (!meshInfo.has_value()) {
            LOG_WARN("Failed to load mesh '{}'", filename);
        }

        return meshInfo;
    }

    /*GeometryHandle GeometryManager::create(const GeometryCreateInfo& info) {
        uint32_t id;

        if (freeIds.empty()) {
            id = nextId++;
            entries.resize(nextId);
        } else {
            id = freeIds.back();
            freeIds.pop_back();
        }

        entries[id] = std::make_unique<GeometryComponent>(devices, info);
        return GeometryHandle{id};
    }*/

    GeometryManager::~GeometryManager() {
        if (entries.empty()) return;
        entries.clear();
    }

    void GeometryManager::destroy(const GeometryHandle& handle) {
        if (handle.id >= entries.size()) return;

        entries[handle.id].reset();

        freeIds.push_back(handle.id);
    }
}