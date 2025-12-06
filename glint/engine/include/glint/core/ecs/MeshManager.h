#pragma once

#include <cstdint>
#include <vector>

namespace glint::engine::scene::components {
    struct MeshCreateInfo;
    struct MeshComponent;
}

namespace glint::engine::core {
    struct MeshHandle {
        uint32_t id;
        uint32_t version;

      public:
        MeshHandle() = delete;
        MeshHandle(uint32_t id_) : id(id_), version(0){};

        bool operator==(const MeshHandle& other) const { return id == other.id && version == other.version; }
    };

    struct MeshManager {
      private:
        std::vector<std::unique_ptr<scene::components::MeshComponent>> meshes;

        std::vector<uint32_t> pool = {};
        uint32_t next = 0;

      public:
        MeshManager() = default;
        ~MeshManager();

        MeshHandle create(const scene::components::MeshCreateInfo& info);
        void destroy(MeshHandle handle);

        inline const scene::components::MeshComponent* get(MeshHandle handle) const {
            if (handle.id >= meshes.size()) return nullptr;
            return meshes[handle.id].get();
        }
    };
}