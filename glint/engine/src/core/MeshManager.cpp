#include "glint/core/MeshManager.h"
#include "glint/scene/components/MeshComponent.h"

namespace glint::engine::core {
    using namespace scene::components;

    MeshHandle MeshManager::create(const MeshCreateInfo& info) {
        uint32_t id;
        if (!pool.empty()) {
            id = pool.back();
            pool.pop_back();
        } else {
            id = next++;
            meshes.resize(next);
        }

        meshes[id] = std::make_unique<MeshComponent>(info);
        return MeshHandle{id};
    }

    ~MeshManager::MeshManager() {

        for (int i = 0; i < meshes.size(); ++i) {
            auto& mesh = meshes[i];

            vkDestroyBuffer(device, mesh->vertices->value, nullptr);
            vkFreeMemory(device, mesh->vertices->memory, nullptr);

            vkDestroyBuffer(device, mesh->indices->value, nullptr);
            vkFreeMemory(device, mesh->indices->memory, nullptr);
        }
    }

    void MeshManager::destroy(MeshHandle handle) {
        auto& mesh = meshes[handle.id];

        mesh.reset();

        pool.push_back(handle.id);
    }
}