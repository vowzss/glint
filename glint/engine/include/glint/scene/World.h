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
        scene::Transform* transform = nullptr;
        scene::GeometryComponent* geometry = nullptr;
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

        // --- geometry ---
        core::GeometryHandle createGeometry(const graphics::Devices& devices, const std::string& path);
        void destroyGeometry(core::GeometryHandle handle) noexcept;

        EntityView entity(core::EntityHandle handle);
        std::vector<EntityView> entities();
    };

}