#pragma once

#include "ComponentStorage.h"

namespace glint::engine::core {

    class TransformComponent;
    struct GeometryComponent;

    class ComponentManager {
        ComponentStorage<TransformComponent> m_transforms;
        ComponentStorage<GeometryComponent> m_geometries;

      public:
        ComponentManager() noexcept = default;
        ~ComponentManager() noexcept = default;

        ComponentManager(const ComponentManager&) = delete;
        ComponentManager& operator=(const ComponentManager&) = delete;

        ComponentManager(ComponentManager&&) = delete;
        ComponentManager& operator=(ComponentManager&&) = delete;

        // --- methods ---
        template <typename T>
        inline void add(EntityHandle handle, T&& component);

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