#include <memory>

#include "glint/core/ecs/ComponentManager.h"
#include "glint/core/ecs/EntityManager.h"
#include "glint/core/ecs/GeometryManager.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/scene/World.h"
#include "glint/scene/components/GeometryComponent.h"
#include "glint/scene/components/Transform.h"

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
        EntityHandle handle = m_entities->create();
        m_components->add(handle, Transform{});

        return handle;
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

        auto& geometries = m_components->query<GeometryComponent>();
        geometries.each([handle](GeometryComponent& component) {
            if (!component.matches(handle)) return;
            component.invalidate();
        });
    }

    EntityView World::entity(EntityHandle handle) {
        EntityView view;

        if (!m_entities->isAlive(handle)) {
            return view;
        }

        view.transform = m_components->get<Transform>(handle);
        view.geometry = m_components->get<GeometryComponent>(handle);

        return view;
    }

}
