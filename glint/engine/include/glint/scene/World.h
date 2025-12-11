#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glint/core/ecs/ObjectHandle.h"

namespace glint::engine {
    namespace core {
        struct EntityManager;
        struct GeometryManager;
        struct ComponentManager;

        template <typename Component>
        class ComponentStorage;

        struct AssetManager;
    }

    namespace graphics {
        struct GeometryData;
        struct Devices;
    }

    namespace scene {
        struct Transform;
        struct GeometryComponent;
    }
}

namespace glint::engine::scene {

    struct EntityView {
        const scene::Transform* transform = nullptr;
        const scene::GeometryComponent* geometry = nullptr;
    };

    class World {
        std::unique_ptr<core::EntityManager> m_entities;
        std::unique_ptr<core::GeometryManager> m_geometries;
        std::unique_ptr<core::ComponentManager> m_components;

      public:
        World(core::AssetManager* assets);
        ~World();

        // --- entities ---
        core::EntityHandle createEntity() noexcept;
        void destroyEntity(core::EntityHandle handle) noexcept;

        // --- geometries ---
        core::GeometryHandle createGeometry(const graphics::Devices& devices, const std::string& path);
        void destroyGeometry(core::GeometryHandle handle) noexcept;

        // --- components ---
        template <typename Component>
        void attach(core::EntityHandle handle, const Component& component) const noexcept;

        template <typename Component>
        void detach(core::EntityHandle handle) const noexcept;

        template <typename Component>
        const Component* component(core::EntityHandle handle) const noexcept;

        // --- query ---
        template <typename Component, typename Handle>
        const Component* resolve(Handle handle) const noexcept;

        EntityView entity(core::EntityHandle handle) const noexcept;
        std::vector<EntityView> entities() const;
    };

}