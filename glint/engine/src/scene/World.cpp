#include <memory>

#include "glint/core/ecs/ComponentManager.h"
#include "glint/core/ecs/EntityManager.h"
#include "glint/core/ecs/GeometryManager.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/scene/World.h"
#include "glint/scene/components/GeometryComponent.h"

using namespace glint::engine::core;
using namespace glint::engine::graphics;

namespace glint::engine::scene {

    World::World(AssetManager* assets) {
        m_entities = std::make_unique<EntityManager>();
        m_geometries = std::make_unique<GeometryManager>(assets);
        m_components = std::make_unique<ComponentManager>();
    }

    World::~World() = default;

    // --- entities ---
    EntityHandle World::createEntity() noexcept {
        return m_entities->create();
    }

    void World::destroyEntity(EntityHandle handle) noexcept {
        m_components->removeAll(handle);
        m_entities->destroy(handle);
    }

    // --- geometry ---
    GeometryHandle World::createGeometry(const Devices& devices, const std::string& path) {
        return m_geometries->create(devices, path);
    }

    void World::destroyGeometry(GeometryHandle handle) noexcept {
        m_geometries->destroy(handle);

        auto& storage = getStorage<GeometryComponent>();
        storage.each([&](GeometryComponent& component) {
            if (component.handle != handle) return;
            component.handle = GeometryHandle::invalid();
        });
    }
}
