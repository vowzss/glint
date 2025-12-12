#include <memory>

#include "glint/core/ecs/ComponentManager.h"
#include "glint/core/ecs/EntityManager.h"
#include "glint/core/ecs/GeometryManager.h"
#include "glint/core/ecs/World.h"
#include "glint/core/ecs/components/GeometryComponent.h"
#include "glint/core/ecs/components/TransformComponent.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/graphics/backend/buffer/GeometryBufferObject.h"

using namespace glint::engine::graphics;

namespace glint::engine::core {

    World::World(AssetManager& assets) {
        m_entities = std::make_unique<EntityManager>();
        m_geometries = std::make_unique<GeometryManager>(assets);
        m_components = std::make_unique<ComponentManager>();
    }

    World::~World() = default;

    // --- entities ---
    EntityHandle World::createEntity() noexcept {
        EntityHandle handle = m_entities->create();
        m_components->add(handle, TransformComponent{});
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

    // --- components ---
    template <typename Component>
    void World::attach(core::EntityHandle handle, Component&& component) const noexcept {
        if (!m_entities->isValid(handle)) return;
        m_components->add<Component>(handle, std::forward<Component>(component));
    }

    template void World::attach<TransformComponent>(core::EntityHandle, TransformComponent&&) const noexcept;
    template void World::attach<GeometryComponent>(core::EntityHandle, GeometryComponent&&) const noexcept;

    template <typename Component>
    void World::detach(core::EntityHandle handle) const noexcept {
        if (!m_entities->isValid(handle)) return;
        m_components->remove<Component>(handle);
    }

    template <typename Component>
    const Component* World::component(core::EntityHandle handle) const noexcept {
        if (!m_entities->isValid(handle)) return nullptr;
        return m_components ? m_components->get<Component>(handle) : nullptr;
    }

    template const TransformComponent* World::component<TransformComponent>(core::EntityHandle) const noexcept;
    template const GeometryComponent* World::component<GeometryComponent>(core::EntityHandle) const noexcept;

    // --- query ---
    template <typename Component, typename Handle>
    const Component* World::resolve(Handle) const noexcept {
        static_assert(sizeof(Handle) == 0, "Unsupported handle type!");
        return nullptr;
    }

    template <>
    const graphics::GeometryBufferObject* World::resolve<graphics::GeometryBufferObject, core::GeometryHandle>(
        core::GeometryHandle handle) const noexcept {
        return m_geometries ? m_geometries->get(handle) : nullptr;
    }

    EntityView World::entity(EntityHandle handle) const noexcept {
        if (!m_entities->isValid(handle)) {
            return EntityView{};
        }

        EntityView view;
        view.transform = m_components->get<TransformComponent>(handle);
        view.geometry = m_components->get<GeometryComponent>(handle);

        return view;
    }

    std::vector<EntityView> World::entities() const {
        const std::vector<EntityHandle>& handles = m_entities->query();

        std::vector<EntityView> entities;
        entities.reserve(handles.size());

        for (const EntityHandle& handle : handles) {
            EntityView view;
            view.transform = m_components->get<TransformComponent>(handle);
            view.geometry = m_components->get<GeometryComponent>(handle);

            entities.emplace_back(view);
        }

        return entities;
    }

}
