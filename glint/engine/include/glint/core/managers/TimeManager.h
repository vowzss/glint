#pragma once

namespace glint::engine::core {

    class TimeManager {
        float m_delta = 0.f;
        float m_scale = 1.0f;
        float m_total = 0.f;

      public:
        inline void tick(float delta) {
            m_delta = delta * m_scale;
            m_total += m_delta;
        }

        // --- getters ---
        inline float deltaTime() const noexcept {
            return m_delta;
        }
        inline float totalTime() const noexcept {
            return m_total;
        }

        // --- setters ---
        inline void setTimeScale(float scale) noexcept {
            m_scale = scale;
        }
    };

}