#include "glint/core/AssetManager.h"
#include "glint/core/Logger.h"
#include "glint/graphics/models/GeometryData.h"
#include "glint/graphics/models/GeometryLoader.h"

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
        using namespace graphics::models;

        std::unique_ptr<Asset> asset;
        if constexpr (std::is_same_v<Asset, GeometryData>) {
            if (auto data = GeometryLoader::load(path); data.has_value()) {
                asset = std::make_unique<Asset>(std::move(data.value()));
            }
        }

        if (asset == nullptr) {
            LOG_WARN("Failed to load asset! path=[{}]", path);
            return AssetHandle<Asset>::invalid();
        }

        uint32_t id = computeUniqueId();
        entries[id].data = asset.release();
        entries[id].deleter = [](void* ptr) { delete static_cast<Asset*>(ptr); };
        entries[id].version++;

        LOG_INFO("Successfully loaded asset. path=[{}]", path);

        return AssetHandle<Asset>{id, entries[id].version};
    }

    template <typename Asset>
    void AssetManager::unload(AssetHandle<Asset> handle) noexcept {
        if (!handle.isValid() || handle.id >= entries.size()) {
            return;
        }

        AssetEntry& entry = entries[handle.id];
        if (entry.version != handle.version) {
            return;
        }

        entry.deleter(entry.data);

        entry.data = nullptr;
        entry.deleter = nullptr;
        entry.version++;

        freeIds.push_back(handle.id);
    }

    template <typename Asset>
    bool AssetManager::exists(AssetHandle<Asset> handle) const noexcept {
        if (!handle.isValid() || handle.id >= entries.size()) {
            return false;
        }

        const AssetEntry& entry = entries[handle.id];
        return entry.version == handle.version && entry.data != nullptr;
    }

    template <typename Asset>
    const Asset* AssetManager::get(AssetHandle<Asset> handle) const noexcept {
        if (!handle.isValid() || handle.id >= entries.size()) {
            return nullptr;
        }

        const AssetEntry& entry = entries[handle.id];
        if (entry.version != handle.version) {
            return nullptr;
        }

        return static_cast<const Asset*>(entry.data);
    }

    uint32_t AssetManager::computeUniqueId() {
        const uint32_t id = freeIds.empty() ? nextId++ : freeIds.back();

        if (!freeIds.empty()) {
            freeIds.pop_back();
        }

        if (id >= entries.size()) {
            entries.resize(id + 1);
        }

        return id;
    }

}
