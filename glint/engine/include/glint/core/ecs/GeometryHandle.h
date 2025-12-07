#pragma once

#include <cstdint>

namespace glint::engine::core {

    struct GeometryHandle {
      public:
        uint32_t id;
        uint32_t version;

      public:
        GeometryHandle() = delete;
        constexpr GeometryHandle(uint32_t id_, uint32_t version_) : id(id_), version(version_) {};

      public:
        inline bool isValid() const noexcept {
            return id != UINT32_MAX;
        }

        // --- operators ---
        constexpr bool operator==(const GeometryHandle& other) const noexcept {
            return id == other.id && version == other.version;
        }

        constexpr bool operator!=(const GeometryHandle& other) const noexcept {
            return !(*this == other);
        }

        // --- factories ---
        static inline constexpr GeometryHandle invalid() noexcept {
            return GeometryHandle{UINT32_MAX, 0};
        }
    };

}