#include "glint/core/AssetManager.h"
#include "glint/loaders/GeometryLoader.h"
#include "glint/scene/components/GeometryComponent.h"

namespace glint::engine::core {

    AssetManager::~AssetManager() {
        for (size_t i = 0; i < entries.size(); ++i) {
            AssetEntry asset = entries[i];
            if (!asset.data || !asset.deleter) continue;

            asset.deleter(asset.data);
        }
    }

    template <typename Asset>
    AssetHandle<Asset> AssetManager::load(const std::string& path) {
        using namespace scene::components;
        using namespace loaders;

        std::unique_ptr<Asset> asset;

        if constexpr (std::is_same_v<Asset, GeometryData>) {
            auto data = GeometryLoader::load(path);
            if (!data.has_value()) {
                return AssetHandle<Asset>::invalid();
            }

            asset = std::make_unique<Asset>(std::move(data.value()));
        }

        if (asset == nullptr) {
            return AssetHandle<Asset>::invalid();
        }

        uint32_t id;
        if (freeIds.empty()) {
            id = nextId++;
            entries.resize(nextId);
        } else {
            id = freeIds.back();
            freeIds.pop_back();
        }

        entries[id].data = asset.release();
        entries[id].version++;
        entries[id].deleter = [](void* ptr) { delete static_cast<Asset*>(ptr); };

        return AssetHandle<Asset>{id, entries[id].version};
    }

    template <typename Asset>
    bool AssetManager::exists(AssetHandle<Asset> handle) const {
        if (!handle.isValid() || handle.id >= entries.size()) {
            return false;
        }

        const AssetEntry& entry = entries[handle.id];
        return entry.version == handle.version && entry.data != nullptr;
    }

    template <typename Asset>
    const Asset* AssetManager::get(AssetHandle<Asset> handle) const {
        if (!handle.isValid() || handle.id >= entries.size()) {
            return nullptr;
        }

        const AssetEntry& entry = entries[handle.id];
        if (entry.version != handle.version) {
            return nullptr;
        }

        return static_cast<const Asset*>(entry.data);
    }

}
