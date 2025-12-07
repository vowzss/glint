namespace glint::engine::scene {

    template <typename Component>
    core::ComponentStorage<Component>& World::getStorage() noexcept {
        return componentManager->getStorage<Component>();
    }

    template <typename Component>
    const core::ComponentStorage<Component>& World::getStorage() const noexcept {
        return componentManager->getStorage<Component>();
    }
}
