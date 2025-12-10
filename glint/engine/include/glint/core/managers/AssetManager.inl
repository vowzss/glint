namespace glint::engine::core {

    template <typename Asset>
    AssetHandle<Asset> AssetManager::load(const std::string& path) {
        // Example implementation:
        uint32_t id = computeUniqueId();

        // Here you'd actually load the asset from 'path'
        Asset* asset = new Asset(); // placeholder

        AssetEntry entry{};
        entry.data = static_cast<void*>(asset);
        entry.deleter = [](void* ptr) { delete static_cast<Asset*>(ptr); };
        entry.version = 1;

        if (id < entries.size()) {
            entries[id] = entry;
        } else {
            entries.push_back(entry);
        }

        return AssetHandle<Asset>{id, entry.version};
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

}
