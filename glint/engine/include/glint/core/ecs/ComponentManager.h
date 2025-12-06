#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "glint/scene/components/Transform.h"

#include "EntityManager.h"

namespace glint::engine::core {

    template <typename Component>
    struct ComponentStorage {
        std::vector<Component> data;
        std::vector<uint32_t> owners;
        std::unordered_map<uint32_t, size_t> lookup;

      public:
        void add(EntityHandle handle, const Component& component) {
            data.push_back(component);
            owners.push_back(handle.id);
            lookup[handle.id] = data.size() - 1;
        }

        Component& get(EntityHandle handle) {
            return data[lookup[handle.id]];
        }

        bool contains(uint32_t id) const {
            return lookup.find(id) != lookup.end();
        }

        void remove(EntityHandle handle) {
            size_t index = lookup[handle.id];
            size_t last = data.size() - 1;

            data[index] = data[last];
            owners[index] = owners[last];
            lookup[owners[index]] = index;

            data.pop_back();
            owners.pop_back();
            lookup.erase(handle.id);
        }
    };

    struct ComponentManager {
      private:
        ComponentStorage<scene::components::Transform> transforms = {};

      public:
        template <typename T>
        inline void add(EntityHandle handle, const T& component) {
            getStorage<T>().add(handle, component);
        }

        template <typename T>
        inline bool has(EntityHandle handle) {
            return getStorage<T>().contains(handle);
        }

        template <typename T>
        inline T& get(EntityHandle handle) {
            return getStorage<T>().get(handle);
        }

        template <typename T>
        inline void remove(EntityHandle handle) {
            getStorage<T>().remove(handle);
        }

        inline void removeAll(EntityHandle handle) {
            transforms.remove(handle);
        }

      private:
        template <typename T>
        ComponentStorage<T>& getStorage();
    };
}