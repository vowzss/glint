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
      private:
        using Devices = graphics::Devices;

      private:
        std::unique_ptr<core::EntityManager> entityManager;
        std::unique_ptr<core::GeometryManager> geometryManager;
        std::unique_ptr<core::ComponentManager> componentManager;

      public:
        World(core::AssetManager* assetManager);
        ~World();

      public:
        // --- entities ---
        core::EntityHandle createEntity() noexcept;
        void destroyEntity(core::EntityHandle handle) noexcept;

        // --- geometry ---
        core::GeometryHandle createGeometry(const Devices& devices, const std::string& path);
        void destroyGeometry(core::GeometryHandle handle) noexcept;

        // --- components ---
        template <typename Component>
        core::ComponentStorage<Component>& getStorage() noexcept;

        template <typename Component>
        const core::ComponentStorage<Component>& getStorage() const noexcept;
    };

}

#include "World.inl"