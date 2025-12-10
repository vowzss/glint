namespace glint::engine::core {

    template <typename Asset>
    AssetHandle<Asset> AssetManager::load(const std::string& path) {
        const uint32_t id = m_freeIds.empty() ? m_nextId++ : m_freeIds.back();
        if (!m_freeIds.empty()) m_freeIds.pop_back();
        if (id >= m_entries.size()) m_entries.resize(id + 1);

        Asset* asset = nullptr;

        if constexpr (std::is_same_v<Asset, graphics::GeometryData>) {
            auto data = graphics::GeometryLoader::load(path);
            if (!data.has_value()) return AssetHandle<Asset>::invalid();

            asset = new Asset(std::move(data.value()));
        }

        if (asset == nullptr) {
            throw std::runtime_error("AssetManager::load: unsupported asset type");
        }

        AssetEntry entry{};
        entry.data = static_cast<void*>(asset);
        entry.deleter = [](void* ptr) { delete static_cast<Asset*>(ptr); };
        entry.version = 1;

        if (id < m_entries.size()) {
            m_entries[id] = entry;
        } else {
            m_entries.push_back(entry);
        }

        return AssetHandle<Asset>{id, entry.version};
    }

    template <typename Asset>
    void AssetManager::unload(AssetHandle<Asset> handle) noexcept {
        if (!handle.isValid() || handle.id >= m_entries.size()) {
            return;
        }

        AssetEntry& entry = m_entries[handle.id];
        if (entry.version != handle.version) {
            return;
        }

        entry.deleter(entry.data);

        entry.data = nullptr;
        entry.deleter = nullptr;
        entry.version++;

        m_freeIds.push_back(handle.id);
    }

    template <typename Asset>
    bool AssetManager::exists(AssetHandle<Asset> handle) const noexcept {
        if (!handle.isValid() || handle.id >= m_entries.size()) {
            return false;
        }

        const AssetEntry& entry = m_entries[handle.id];
        return entry.version == handle.version && entry.data != nullptr;
    }

    template <typename Asset>
    const Asset* AssetManager::get(AssetHandle<Asset> handle) const noexcept {
        if (!handle.isValid() || handle.id >= m_entries.size()) {
            return nullptr;
        }

        const AssetEntry& entry = m_entries[handle.id];
        if (entry.version != handle.version) {
            return nullptr;
        }

        return static_cast<const Asset*>(entry.data);
    }
}
