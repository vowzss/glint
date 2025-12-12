#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glint/core/ecs/ObjectHandle.h"

namespace glint::engine {
    namespace graphics {
        struct GeometryData;
        struct Devices;
    }
}

namespace glint::engine::core {

    class EntityManager;
    class GeometryManager;
    class ComponentManager;

    template <typename Component>
    class ComponentStorage;

    class AssetManager;

    class TransformComponent;
    struct GeometryComponent;

    struct EntityView {
        const TransformComponent* transform = nullptr;
        const GeometryComponent* geometry = nullptr;
    };

    class World {
        std::unique_ptr<EntityManager> m_entities;
        std::unique_ptr<GeometryManager> m_geometries;
        std::unique_ptr<ComponentManager> m_components;

      public:
        World(AssetManager& assets);
        ~World();

        // --- entities ---
        EntityHandle createEntity() noexcept;
        void destroyEntity(EntityHandle handle) noexcept;

        // --- geometries ---
        GeometryHandle createGeometry(const graphics::Devices& devices, const std::string& path);
        void destroyGeometry(GeometryHandle handle) noexcept;

        // --- components ---
        template <typename Component>
        void attach(EntityHandle handle, Component&& component) const noexcept;

        template <typename Component>
        void detach(EntityHandle handle) const noexcept;

        template <typename Component>
        const Component* component(EntityHandle handle) const noexcept;

        // --- query ---
        template <typename Component, typename Handle>
        const Component* resolve(Handle handle) const noexcept;

        EntityView entity(EntityHandle handle) const noexcept;
        std::vector<EntityView> entities() const;
    };

}