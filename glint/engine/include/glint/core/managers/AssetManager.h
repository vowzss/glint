#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "glint/core/ecs/ObjectHandle.h"

namespace glint::engine::core {

    struct AssetEntry {
        uint32_t version = 0;
        std::unique_ptr<void, void (*)(void*)> data = {nullptr, nullptr};

      public:
        AssetEntry() noexcept = default;

        AssetEntry(const AssetEntry&) = delete;
        AssetEntry& operator=(const AssetEntry&) = delete;

        AssetEntry(AssetEntry&&) noexcept = default;
        AssetEntry& operator=(AssetEntry&&) noexcept = default;
    };

    class AssetManager {
        std::vector<AssetEntry> m_entries;

        std::vector<uint32_t> m_freeIds;
        uint32_t m_nextId = 0;

      public:
        AssetManager() noexcept = default;
        ~AssetManager() noexcept = default;

        // --- methods ---
        template <typename Asset>
        AssetHandle<Asset> load(const std::string& path);

        template <typename Asset>
        void unload(AssetHandle<Asset> handle) noexcept;

        // --- getters ---
        template <typename Asset>
        bool exists(AssetHandle<Asset> handle) const noexcept;

        template <typename Asset>
        const Asset* get(AssetHandle<Asset> handle) const noexcept;
    };
}