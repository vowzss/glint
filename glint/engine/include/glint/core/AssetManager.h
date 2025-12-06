#pragma once

#include <string>
#include <vector>

namespace glint::engine::core {

    template <typename T>
    struct AssetHandle {
        uint32_t id;
        uint32_t version;

      public:
        static constexpr AssetHandle<T> invalid() {
            return AssetHandle<T>{UINT32_MAX, 0};
        }

        bool isValid() const {
            return id != UINT32_MAX;
        }
    };

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
        bool exists(AssetHandle<Asset> handle) const;

        template <typename Asset>
        const Asset* get(AssetHandle<Asset> handle) const;
    };

}