#include <stdexcept>

#include "glint/core/managers/AssetManager.h"
#include "glint/graphics/assets/Geometry.h"
#include "glint/graphics/assets/GeometryLoader.h"

using namespace glint::engine::graphics;

namespace glint::engine::core {

    template <typename Asset>
    AssetHandle<Asset> AssetManager::load(const std::string& path) {
        const uint32_t id = m_freeIds.empty() ? m_nextId++ : m_freeIds.back();
        if (!m_freeIds.empty()) m_freeIds.pop_back();
        if (id >= m_entries.size()) m_entries.resize(id + 1);

        Asset* asset = nullptr;

        if constexpr (std::is_same_v<Asset, graphics::Geometry>) {
            auto data = graphics::GeometryLoader::load(path);
            if (!data.has_value()) return AssetHandle<Asset>::invalid();

            asset = new Asset(std::move(data.value()));
        }

        if (asset == nullptr) {
            throw std::runtime_error("AssetManager::load: unsupported asset type");
        }

        AssetEntry entry = {};
        entry.data = std::unique_ptr<void, void (*)(void*)>(static_cast<void*>(asset), [](void* ptr) { delete static_cast<Asset*>(ptr); });
        entry.version = 1;

        if (id >= m_entries.size()) m_entries.resize(id + 1);
        m_entries[id] = std::move(entry);

        return AssetHandle<Asset>{id, entry.version};
    }

    template <typename Asset>
    void AssetManager::unload(AssetHandle<Asset> handle) noexcept {
        if (!handle.valid() || handle.id() >= m_entries.size()) {
            return;
        }

        AssetEntry& entry = m_entries[handle.id()];
        if (entry.version != handle.version()) {
            return;
        }

        entry.data.reset();
        entry.version++;

        m_freeIds.push_back(handle.id());
    }

    template <typename Asset>
    bool AssetManager::exists(AssetHandle<Asset> handle) const noexcept {
        if (!handle.valid() || handle.id() >= m_entries.size()) {
            return false;
        }

        const AssetEntry& entry = m_entries[handle.id()];
        return entry.version == handle.version() && entry.data != nullptr;
    }

    template <typename Asset>
    const Asset* AssetManager::get(AssetHandle<Asset> handle) const noexcept {
        if (!handle.valid() || handle.id() >= m_entries.size()) {
            return nullptr;
        }

        const AssetEntry& entry = m_entries[handle.id()];
        if (entry.version != handle.version()) {
            return nullptr;
        }

        return static_cast<const Asset*>(entry.data.get());
    }

    template AssetHandle<graphics::Geometry> AssetManager::load<graphics::Geometry>(const std::string& path);
    template void AssetManager::unload<graphics::Geometry>(AssetHandle<graphics::Geometry> handle) noexcept;

    template bool AssetManager::exists<graphics::Geometry>(AssetHandle<graphics::Geometry> handle) const noexcept;
    template const graphics::Geometry* AssetManager::get<graphics::Geometry>(AssetHandle<graphics::Geometry> handle) const noexcept;
}