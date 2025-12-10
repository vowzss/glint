#include <memory>

#include "glint/core/ecs/ComponentManager.h"
#include "glint/core/ecs/EntityManager.h"
#include "glint/core/ecs/GeometryManager.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/scene/World.h"
#include "glint/scene/components/GeometryComponent.h"

using namespace glint::engine::core;

namespace glint::engine::scene {

    World::World(AssetManager* assetManager) {
        entityManager = std::make_unique<EntityManager>();
        geometryManager = std::make_unique<GeometryManager>(assetManager);
        componentManager = std::make_unique<ComponentManager>();
    }

    World::~World() = default;

    // --- entities ---
    EntityHandle World::createEntity() noexcept {
        return entityManager->create();
    }

    void World::destroyEntity(EntityHandle handle) noexcept {
        componentManager->removeAll(handle);
        entityManager->destroy(handle);
    }

    // --- geometry ---
    GeometryHandle World::createGeometry(const Devices& devices, const std::string& path) {
        return geometryManager->create(devices, path);
    }

    void World::destroyGeometry(GeometryHandle handle) noexcept {
        geometryManager->destroy(handle);

        auto& storage = getStorage<GeometryComponent>();
        storage.each([&](GeometryComponent& component) {
            if (component.handle != handle) return;
            component.handle = GeometryHandle::invalid();
        });
    }
}
