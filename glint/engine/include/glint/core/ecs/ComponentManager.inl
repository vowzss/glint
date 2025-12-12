namespace glint::engine::core {

    template <typename T>
    inline void ComponentManager::add(EntityHandle handle, T&& component) {
        query<T>().add(handle, std::forward<T>(component));
    }

    template <typename T>
    inline bool ComponentManager::has(EntityHandle handle) const noexcept {
        return query<T>().contains(handle.id());
    }

    template <typename T>
    inline T* ComponentManager::get(EntityHandle handle) {
        return query<T>().get(handle);
    }

    template <typename T>
    inline const T* ComponentManager::get(EntityHandle handle) const {
        return query<T>().get(handle);
    }

    template <typename T>
    inline void ComponentManager::remove(EntityHandle handle) {
        query<T>().remove(handle);
    }

    inline void ComponentManager::removeAll(EntityHandle handle) {
        if (!m_transforms.contains(handle.id())) return;

        m_transforms.remove(handle);
        m_geometries.remove(handle);
    }

    // clang-format off
    template <>
    inline ComponentStorage<TransformComponent>& 
    ComponentManager::query<TransformComponent>() noexcept {
        return m_transforms;
    };

    template <>
    inline const ComponentStorage<TransformComponent>& 
    ComponentManager::query<TransformComponent>() const noexcept {
        return m_transforms;
    };

    template <>
    inline ComponentStorage<core::GeometryComponent>&
    ComponentManager::query<core::GeometryComponent>() noexcept {
        return m_geometries;
    }

    template <>
    inline const ComponentStorage<core::GeometryComponent>& 
    ComponentManager::query<core::GeometryComponent>() const noexcept {
        return m_geometries;
    }
    // clang-format on
}