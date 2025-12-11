#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "ObjectHandle.h"

namespace glint::engine::core {

    template <typename Component>
    class ComponentStorage {
        std::vector<Component> m_data;
        std::vector<uint32_t> m_owners;
        std::unordered_map<uint32_t, size_t> m_lookup;

      public:
        ComponentStorage() noexcept = default;
        ~ComponentStorage() noexcept = default;

        ComponentStorage(const ComponentStorage&) = delete;
        ComponentStorage& operator=(const ComponentStorage&) = delete;

        ComponentStorage(ComponentStorage&&) = delete;
        ComponentStorage& operator=(ComponentStorage&&) = delete;

        // --- methods ---
        inline void add(EntityHandle handle, const Component& component) noexcept;

        inline Component* get(EntityHandle handle) noexcept;
        inline const Component* get(EntityHandle handle) const noexcept;

        inline bool contains(uint32_t id) const noexcept;

        inline void remove(EntityHandle handle) noexcept;

        template <typename Func>
        inline void each(Func&& func) noexcept(noexcept(func(std::declval<Component&>())));

        template <typename Func>
        inline void each(Func&& func) const noexcept(noexcept(func(std::declval<const Component&>())));

        inline size_t size() const noexcept;
    };

}

#include "ComponentStorage.inl"