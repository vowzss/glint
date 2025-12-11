#pragma once

#include <cstdint>

namespace glint::engine::core {

    template <typename Tag>
    class ObjectHandle {
        uint32_t m_id = 0;
        uint32_t m_version = 0;

      public:
        ObjectHandle() noexcept = default;
        explicit ObjectHandle(uint32_t id, uint32_t version) noexcept : m_id(id), m_version(version) {};
        explicit ObjectHandle(uint32_t id) noexcept : m_id(id), m_version(0) {};

        ~ObjectHandle() noexcept = default;

        // --- factories ---
        static inline constexpr ObjectHandle invalid() noexcept {
            return ObjectHandle{0, 0};
        }

        // --- methods ---
        constexpr bool valid() const noexcept {
            return m_id != 0;
        }

        // --- getters ---
        const uint32_t& id() const noexcept {
            return m_id;
        }

        const uint32_t& version() const noexcept {
            return m_version;
        }

        // --- operators ---
        constexpr bool operator==(const ObjectHandle& other) const noexcept {
            return m_id == other.m_id && m_version == other.m_version;
        }

        constexpr bool operator!=(const ObjectHandle& other) const noexcept {
            return !(*this == other);
        }
    };

    using EntityHandle = ObjectHandle<struct EntityTag>;
    using GeometryHandle = ObjectHandle<struct GeometryTag>;

    template <typename AssetType>
    struct AssetTag {};

    template <typename AssetType>
    using AssetHandle = ObjectHandle<struct AssetTag<AssetType>>;
}