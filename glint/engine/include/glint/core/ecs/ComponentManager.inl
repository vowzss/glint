namespace glint::engine::core {

    template <typename T>
    inline void ComponentManager::add(EntityHandle handle, const T& component) {
        getStorage<T>().add(handle, component);
    }

    template <typename T>
    inline bool ComponentManager::has(EntityHandle handle) const noexcept {
        return getStorage<T>().contains(handle.id);
    }

    template <typename T>
    inline T& ComponentManager::get(EntityHandle handle) {
        return getStorage<T>().get(handle);
    }

    template <typename T>
    inline const T& ComponentManager::get(EntityHandle handle) const {
        return getStorage<T>().get(handle);
    }

    template <typename T>
    inline void ComponentManager::remove(EntityHandle handle) {
        getStorage<T>().remove(handle);
    }

    inline void ComponentManager::removeAll(EntityHandle handle) {
        if (transforms.contains(handle.id)) {
            transforms.remove(handle);
        }
    }

    // clang-format off
    template <>
    inline ComponentStorage<scene::components::Transform>& 
    ComponentManager::getStorage<scene::components::Transform>() noexcept {
        return transforms;
    };

    template <>
    inline const ComponentStorage<scene::components::Transform>& 
    ComponentManager::getStorage<scene::components::Transform>() const noexcept {
        return transforms;
    };

    template <>
    inline ComponentStorage<scene::components::GeometryComponent>& 
    ComponentManager::getStorage<scene::components::GeometryComponent>() noexcept {
        return geometries;
    }

    template <>
    inline const ComponentStorage<scene::components::GeometryComponent>& 
    ComponentManager::getStorage<scene::components::GeometryComponent>() const noexcept {
        return geometries;
    }

    // clang-format on
}