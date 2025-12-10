#pragma once

namespace glint::engine::core {

    class TimeManager {
        float m_delta;
        float m_scale;
        float m_total;

      public:
        TimeManager(float fixedStep = 1.0f / 60.0f) : m_delta(0), m_scale(1.0f), m_total(0) {
        }

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