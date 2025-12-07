#pragma once

#include <string>
#include <vector>

#include "AssetHandle.h"

namespace glint::engine::core {

    class AssetManager {
      private:
        struct AssetEntry {
            uint32_t version = 0;

            void* data = nullptr;
            void (*deleter)(void*) = nullptr;
        };

        std::vector<AssetEntry> entries;

        std::vector<uint32_t> freeIds;
        uint32_t nextId = 0;

      public:
        AssetManager() = default;
        ~AssetManager();

      public:
        template <typename Asset>
        AssetHandle<Asset> load(const std::string& path);

        template <typename Asset>
        void unload(AssetHandle<Asset> handle) noexcept;

        template <typename Asset>
        bool exists(AssetHandle<Asset> handle) const noexcept;

        template <typename Asset>
        const Asset* get(AssetHandle<Asset> handle) const noexcept;

      private:
        uint32_t computeUniqueId();
    };
}

#include "AssetManager.inl"