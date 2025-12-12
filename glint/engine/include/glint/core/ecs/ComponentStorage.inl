namespace glint::engine::core {

    template <typename Component>
    inline void ComponentStorage<Component>::add(EntityHandle handle, Component&& component) noexcept {
        m_data.push_back(std::move(component));
        m_owners.push_back(handle.id());
        m_lookup[handle.id()] = m_data.size() - 1;
    }

    template <typename Component>
    inline Component* ComponentStorage<Component>::get(EntityHandle handle) noexcept {
        return &m_data[m_lookup.at(handle.id())];
    }

    template <typename Component>
    inline const Component* ComponentStorage<Component>::get(EntityHandle handle) const noexcept {
        return &m_data.at(m_lookup.at(handle.id()));
    }

    template <typename Component>
    inline bool ComponentStorage<Component>::contains(uint32_t id) const noexcept {
        return m_lookup.find(id) != m_lookup.end();
    }

    template <typename Component>
    inline void ComponentStorage<Component>::remove(EntityHandle handle) noexcept {
        size_t index = m_lookup.at(handle.id());
        size_t last = m_data.size() - 1;

        m_data[index] = std::move(m_data[last]);
        m_owners[index] = m_owners[last];
        m_lookup[m_owners[index]] = index;

        m_data.pop_back();
        m_owners.pop_back();
        m_lookup.erase(handle.id());
    }

    template <typename Component>
    template <typename Func>
    inline void ComponentStorage<Component>::each(Func&& func) noexcept(noexcept(func(std::declval<Component&>()))) {
        for (Component& component : m_data) {
            func(component);
        }
    }

    template <typename Component>
    template <typename Func>
    inline void ComponentStorage<Component>::each(Func&& func) const noexcept(noexcept(func(std::declval<const Component&>()))) {
        for (const Component& component : m_data) {
            func(component);
        }
    }

    template <typename Component>
    inline size_t ComponentStorage<Component>::size() const noexcept {
        return m_data.size();
    }

}
