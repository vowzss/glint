#pragma once

#include <cstdint>

namespace glint::engine::core {

    template <typename Tag>
    class ObjectHandle {
        uint32_t m_id = UINT32_MAX;
        uint32_t m_version = 0;

      public:
        ObjectHandle() = default;
        explicit ObjectHandle(uint32_t id, uint32_t version) noexcept : m_id(id), m_version(version) {};
        explicit ObjectHandle(uint32_t id) noexcept : m_id(id), m_version(0) {};

        inline bool valid() const {
            return m_id != UINT32_MAX;
        }

        constexpr bool operator==(const ObjectHandle& other) const noexcept {
            return m_id == other.m_id && m_version == other.m_version;
        }

        constexpr bool operator!=(const ObjectHandle& other) const noexcept {
            return !(*this == other);
        }

        // --- factories ---
        static inline constexpr ObjectHandle invalid() noexcept {
            return ObjectHandle{UINT32_MAX, 0};
        }

        // --- getters ---
        const uint32_t& id() const {
            return m_id;
        }

        const uint32_t& version() const {
            return m_version;
        }
    };

    using EntityHandle = ObjectHandle<struct Entity>;
    using GeometryHandle = ObjectHandle<struct Geometry>;
}