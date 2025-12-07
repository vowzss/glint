#pragma once

#include <memory>

namespace glint::engine {
    namespace graphics::backend {
        struct DeviceContext;
    }

    namespace scene::components {
        struct GeometryCreateInfo;
        struct GeometryComponent;
    }
}

namespace glint::engine::core {
    struct EntityHandle;
    struct EntityManager;

    struct GeometryHandle;
    struct GeometryManager;

    struct ComponentManager;

    template <typename Component>
    class ComponentStorage;

    struct Registry {
        /*private:
          using Devices = graphics::backend::DeviceContext;
          using GeometryCreateInfo = scene::components::GeometryCreateInfo;
          using GeometryComponent = scene::components::GeometryComponent;

        private:
          std::unique_ptr<EntityManager> entities;
          std::unique_ptr<GeometryManager> geometry;
          std::unique_ptr<ComponentManager> components;

        public:
          Registry();

        public:
          // --- entities ---
          EntityHandle createEntity() noexcept;
          void destroyEntity(EntityHandle handle) noexcept;

          // --- geometry ---
          GeometryHandle createGeometry(const GeometryCreateInfo& info);
          const GeometryComponent* getGeometry(const GeometryHandle& handle) noexcept;

          // --- components ---
          template <typename Component>
          void addComponent(EntityHandle handle, const Component& component);

          template <typename Component>
          bool hasComponent(EntityHandle handle) const noexcept;

          template <typename Component>
          Component& getComponent(EntityHandle handle);

          template <typename Component>
          const Component& getComponent(EntityHandle handle) const;

          template <typename Component>
          ComponentStorage<Component>& getStorage() noexcept;

          template <typename Component>
          const ComponentStorage<Component>& getStorage() const noexcept;*/
    };
}