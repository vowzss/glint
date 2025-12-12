#pragma once

namespace glint::engine::core {

    template <typename Handle>
    struct Component {
      protected:
        Handle m_handle;

      public:
        Component() : m_handle(Handle::invalid()) {};
        explicit Component(Handle handle) noexcept : m_handle(handle) {};

        // --- factories ---
        inline void invalidate() noexcept {
            m_handle = Handle::invalid();
        }
        // --- getters ---
        inline bool valid(Handle handle) const noexcept {
            return handle.valid();
        }

        inline bool matches(Handle handle) const noexcept {
            return m_handle == handle;
        }

        inline const Handle& handle() const noexcept {
            return m_handle;
        }

        // --- operators ---
    };

}