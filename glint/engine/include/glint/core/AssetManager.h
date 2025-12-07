#pragma once

#include <string>
#include <vector>

namespace glint::engine::core {

    // todo: maybe put in specific header instead
    template <typename T>
    struct AssetHandle {
        uint32_t id;
        uint32_t version;

      public:
        static inline constexpr AssetHandle<T> invalid() noexcept {
            return AssetHandle<T>{UINT32_MAX, 0};
        }

        inline bool isValid() const noexcept {
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
        void unload(AssetHandle<Asset> handle) noexcept;

        template <typename Asset>
        bool exists(AssetHandle<Asset> handle) const noexcept;

        template <typename Asset>
        const Asset* get(AssetHandle<Asset> handle) const noexcept;

      private:
        uint32_t computeUniqueId();
    };

}