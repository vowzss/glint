#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "glint/graphics/backend/buffer/GeometryBuffer.h"

#include "ObjectHandle.h"

namespace glint::engine::graphics {
    struct Devices;
}

namespace glint::engine::core {

    class AssetManager;

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
            flags = (flags & ~VersionMask) | ((version() + 1) & VersionMask);
        }
    };

    class GeometryManager {
        AssetManager* m_assets;

        std::vector<GeometryEntry> m_entries;

        std::vector<uint32_t> m_freeIds;
        uint32_t m_nextId = 0;

      public:
        GeometryManager(AssetManager* assets) : m_assets(assets){};
        ~GeometryManager() = default;

        GeometryHandle create(const graphics::Devices& devices, const std::string& path);
        void destroy(GeometryHandle handle);

        // --- getters ---
        graphics::GeometryBuffer* get(GeometryHandle handle) noexcept;
        const graphics::GeometryBuffer* get(GeometryHandle handle) const noexcept;

        inline bool isValid(GeometryHandle handle) const noexcept {
            // clang-format off
            return handle.valid() 
              && handle.id() < m_entries.size() 
              && m_entries[handle.id()].version() == handle.version()
              && m_entries[handle.id()].buffer.has_value();
            // clang-format on
        }
    };

}