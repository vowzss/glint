#pragma once

namespace glint::engine::scene {
    struct Transform;
    struct GeometryComponent;
}

#include "ComponentStorage.h"

namespace glint::engine::core {

    struct ComponentManager {
      private:
        ComponentStorage<scene::Transform> transforms;
        ComponentStorage<scene::GeometryComponent> geometries;

      public:
        template <typename T>
        inline void add(EntityHandle handle, const T& component);

        template <typename T>
        inline bool has(EntityHandle handle) const noexcept;

        template <typename T>
        inline T& get(EntityHandle handle);

        template <typename T>
        inline const T& get(EntityHandle handle) const;

        template <typename T>
        inline void remove(EntityHandle handle);

        inline void removeAll(EntityHandle handle);

        template <typename T>
        ComponentStorage<T>& getStorage() noexcept;

        template <typename T>
        const ComponentStorage<T>& getStorage() const noexcept;
    };

}

#include "ComponentManager.inl"