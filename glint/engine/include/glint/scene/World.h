#pragma once

#include <memory>
#include <string>

#include "glint/core/ecs/EntityHandle.h"
#include "glint/core/ecs/GeometryHandle.h"

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
        struct GeometryComponent;
    }
}

namespace glint::engine::scene {

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

        // --- geometry ---
        core::GeometryHandle createGeometry(const graphics::Devices& devices, const std::string& path);
        void destroyGeometry(core::GeometryHandle handle) noexcept;

        // --- components ---
        template <typename Component>
        core::ComponentStorage<Component>& getStorage() noexcept;

        template <typename Component>
        const core::ComponentStorage<Component>& getStorage() const noexcept;
    };

}

#include "World.inl"