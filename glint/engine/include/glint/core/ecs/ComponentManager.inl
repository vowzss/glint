namespace glint::engine::core {

    template <typename T>
    inline void ComponentManager::add(EntityHandle handle, const T& component) {
        query<T>().add(handle, component);
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
    inline ComponentStorage<scene::Transform>& 
    ComponentManager::query<scene::Transform>() noexcept {
        return m_transforms;
    };

    template <>
    inline const ComponentStorage<scene::Transform>& 
    ComponentManager::query<scene::Transform>() const noexcept {
        return m_transforms;
    };

    template <>
    inline ComponentStorage<scene::GeometryComponent>&
    ComponentManager::query<scene::GeometryComponent>() noexcept {
        return m_geometries;
    }

    template <>
    inline const ComponentStorage<scene::GeometryComponent>& 
    ComponentManager::query<scene::GeometryComponent>() const noexcept {
        return m_geometries;
    }
    // clang-format on
}