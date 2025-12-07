#include "glint/core/Registry.h"

namespace glint::engine::core {
    using namespace scene::components;

    /*Registry::Registry(const Devices& devices) {
        entities = std::make_unique<EntityManager>();
        geometry = std::make_unique<GeometryManager>(devices);
        components = std::make_unique<ComponentManager>();
    }

    // --- entities ---
    EntityHandle Registry::createEntity() noexcept {
        return entities->create();
    }

    void Registry::destroyEntity(EntityHandle handle) noexcept {
        components->removeAll(handle);
        entities->destroy(handle);
    }

    // --- geometry ---
    GeometryHandle Registry::createGeometry(const GeometryCreateInfo& info) {
        return geometry->create(info);
    }

    const GeometryComponent* Registry::getGeometry(const GeometryHandle& handle) noexcept {
        return geometry->get(handle);
    }

    // --- components ---
    template <typename Component>
    void Registry::addComponent(EntityHandle handle, const Component& component) {
        components->add(handle, component);
    }

    template <typename Component>
    bool Registry::hasComponent(EntityHandle handle) const noexcept {
        return components->has<Component>(handle);
    }

    template <typename Component>
    Component& Registry::getComponent(EntityHandle handle) {
        return components->get<Component>(handle);
    }

    template <typename Component>
    const Component& Registry::getComponent(EntityHandle handle) const {
        return components->get<Component>(handle);
    }

    template <typename Component>
    ComponentStorage<Component>& Registry::getStorage() noexcept {
        return components->getStorage<Component>();
    }

    template <typename Component>
    const ComponentStorage<Component>& Registry::getStorage() const noexcept {
        return components->getStorage<Component>();
    }*/
}