#pragma once

#include "ComponentStorage.h"

namespace glint::engine::scene {
    class Transform;
    struct GeometryComponent;
}

namespace glint::engine::core {

    class ComponentManager {
        ComponentStorage<scene::Transform> m_transforms;
        ComponentStorage<scene::GeometryComponent> m_geometries;

      public:
        template <typename T>
        inline void add(EntityHandle handle, const T& component);

        template <typename T>
        inline bool has(EntityHandle handle) const noexcept;

        template <typename T>
        inline T* get(EntityHandle handle);

        template <typename T>
        inline const T* get(EntityHandle handle) const;

        template <typename T>
        inline void remove(EntityHandle handle);

        inline void removeAll(EntityHandle handle);

        template <typename T>
        ComponentStorage<T>& query() noexcept;

        template <typename T>
        const ComponentStorage<T>& query() const noexcept;
    };

}

#include "ComponentManager.inl"