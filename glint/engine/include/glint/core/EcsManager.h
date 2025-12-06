#pragma once

#include "glint/core/ecs/ComponentManager.h"
#include "glint/core/ecs/EntityManager.h"
#include "glint/core/ecs/MeshManager.h"

namespace glint::engine::core {

    struct EcsManager {
      private:
        EntityManager entities;
        MeshManager meshes;
        ComponentManager components;

      public:
        EcsManager();
        EntityHandle createEntity() { return entities.create(); }

        void destroyEntity(EntityHandle handle) {
            components.removeAll(handle);
            entities.destroy(handle);
        }

        template <typename Component>
        void addComponent(EntityHandle handle, const Component& c) {
            components.add(handle, c);
        }

        template <typename Component>
        bool hasComponent(EntityHandle handle) {
            return components.has<Component>(handle);
        }

        template <typename Component>
        Component& getComponent(EntityHandle handle) {
            return components.get<Component>(handle);
        }

        template <typename Component>
        ComponentStorage<Component>& getComponentStorage() const {
            return components.getStorage<Component>();
        }

        MeshHandle createMesh(const scene::components::MeshCreateInfo& info) { return meshes.create(info); }
        const scene::components::MeshComponent* getMesh(MeshHandle handle) const { return meshes.get(handle); }
    };
}
