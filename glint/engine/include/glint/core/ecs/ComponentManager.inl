#include "glint/scene/components/GeometryComponent.h"
#include "glint/scene/components/Transform.h"

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
    inline ComponentStorage<scene::Transform>& 
    ComponentManager::getStorage<scene::Transform>() noexcept {
        return transforms;
    };

    template <>
    inline const ComponentStorage<scene::Transform>& 
    ComponentManager::getStorage<scene::Transform>() const noexcept {
        return transforms;
    };

    template <>
    inline ComponentStorage<scene::GeometryComponent>& 
    ComponentManager::getStorage<scene::GeometryComponent>() noexcept {
        return geometries;
    }

    template <>
    inline const ComponentStorage<scene::GeometryComponent>& 
    ComponentManager::getStorage<scene::GeometryComponent>() const noexcept {
        return geometries;
    }

    // clang-format on
}