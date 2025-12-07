#pragma once

#include <cstdint>

namespace glint::engine::core {

    struct EntityHandle {
        size_t id;
        uint32_t version;

      public:
        EntityHandle() = delete;
        constexpr EntityHandle(size_t id_, uint32_t version_) : id(id_), version(version_) {};

      public:
        inline bool isValid() const noexcept {
            return id != UINT32_MAX;
        }

        // --- operators ---
        constexpr bool operator==(const EntityHandle& other) const noexcept {
            return id == other.id && version == other.version;
        }

        constexpr bool operator!=(const EntityHandle& other) const noexcept {
            return !(*this == other);
        }

        // --- factories ---
        static inline constexpr EntityHandle invalid() noexcept {
            return EntityHandle{UINT32_MAX, 0};
        }
    };

}