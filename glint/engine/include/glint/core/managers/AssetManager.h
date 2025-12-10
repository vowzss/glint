#pragma once

#include <string>
#include <vector>

#include "glint/graphics/models/GeometryLoader.h"

#include "AssetHandle.h"


namespace glint::engine::core {

    class AssetManager {
        struct AssetEntry {
            uint32_t version = 0;

            void* data = nullptr;
            void (*deleter)(void*) = nullptr;
        };

        std::vector<AssetEntry> m_entries;

        std::vector<uint32_t> m_freeIds;
        uint32_t m_nextId = 0;

      public:
        AssetManager() = default;
        ~AssetManager();

        template <typename Asset>
        AssetHandle<Asset> load(const std::string& path);

        template <typename Asset>
        void unload(AssetHandle<Asset> handle) noexcept;

        template <typename Asset>
        bool exists(AssetHandle<Asset> handle) const noexcept;

        template <typename Asset>
        const Asset* get(AssetHandle<Asset> handle) const noexcept;
    };
}

#include "AssetManager.inl"