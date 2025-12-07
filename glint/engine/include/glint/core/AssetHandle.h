#pragma once

#include <cstdint>

namespace glint::engine::core {
    template <typename T>
    struct AssetHandle {
        uint32_t id;
        uint32_t version;

      public:
        AssetHandle<T>() = delete;
        constexpr AssetHandle<T>(size_t id_, uint32_t version_) : id(id_), version(version_){};

      public:
        inline bool isValid() const noexcept {
            return id != UINT32_MAX;
        }

        // --- factories ---
        static inline constexpr AssetHandle<T> invalid() noexcept {
            return AssetHandle<T>{UINT32_MAX, 0};
        }
    };
}