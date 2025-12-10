#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "glint/graphics/backend/buffer/GeometryBuffer.h"

#include "ObjectHandle.h"

namespace glint::engine::graphics {
    struct Devices;
}

namespace glint::engine::core {

    struct AssetManager;

    struct GeometryEntry {
      private:
        static constexpr uint32_t VersionMask = 0x7FFFFFFF;

      public:
        uint32_t flags = 0;

        std::optional<graphics::GeometryBuffer> buffer;

      public:
        uint32_t version() const noexcept {
            return flags & VersionMask;
        }

        inline void bump() noexcept {
            uint32_t current = version();
            uint32_t next = current + 1;

            if (next > VersionMask) {
                std::cerr << "[Warning] GeometryEntry version exceeded max value (" << VersionMask << ") and will wrap around.\n";
                next = 0;
            }

            flags = (flags & ~VersionMask) | (next & VersionMask);
        }
    };

    class GeometryManager {
        AssetManager* m_assets;

        std::vector<GeometryEntry> m_entries;

        std::vector<uint32_t> m_freeIds;
        uint32_t m_nextId = 0;

      public:
        GeometryManager(AssetManager* assets) : m_assets(assets) {};
        ~GeometryManager() = default;

        GeometryHandle create(const graphics::Devices& devices, const std::string& path);
        void destroy(GeometryHandle handle);
    };

}