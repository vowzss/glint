namespace glint::engine::core {

    template <typename Component>
    inline void ComponentStorage<Component>::add(EntityHandle handle, const Component& component) noexcept {
        data.push_back(component);
        owners.push_back(handle.id);
        lookup[handle.id] = data.size() - 1;
    }

    template <typename Component>
    inline Component& ComponentStorage<Component>::get(EntityHandle handle) noexcept {
        return data[lookup.at(handle.id)];
    }

    template <typename Component>
    inline const Component& ComponentStorage<Component>::get(EntityHandle handle) const noexcept {
        return data.at(lookup.at(handle.id));
    }

    template <typename Component>
    inline bool ComponentStorage<Component>::contains(uint32_t id) const noexcept {
        return lookup.find(id) != lookup.end();
    }

    template <typename Component>
    inline void ComponentStorage<Component>::remove(EntityHandle handle) noexcept {
        size_t index = lookup.at(handle.id);
        size_t last = data.size() - 1;

        data[index] = std::move(data[last]);
        owners[index] = owners[last];
        lookup[owners[index]] = index;

        data.pop_back();
        owners.pop_back();
        lookup.erase(handle.id);
    }

    template <typename Component>
    template <typename Func>
    inline void ComponentStorage<Component>::each(Func&& func) noexcept(noexcept(func(std::declval<Component&>()))) {
        for (auto& component : data)
            func(component);
    }

    template <typename Component>
    template <typename Func>
    inline void ComponentStorage<Component>::each(Func&& func) const noexcept(noexcept(func(std::declval<const Component&>()))) {
        for (const auto& component : data)
            func(component);
    }

    template <typename Component>
    inline size_t ComponentStorage<Component>::size() const noexcept {
        return data.size();
    }

}
