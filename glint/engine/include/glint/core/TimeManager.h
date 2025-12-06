#pragma once

namespace glint::engine::core {

    struct TimeManager {
      private:
        float delta;
        float scale;
        float total;

      public:
        TimeManager(float fixedStep = 1.0f / 60.0f) : delta(0), scale(1.0f), total(0) {
        }

      public:
        inline void tick(float dt) {
            delta = dt * scale;
            total += delta;
        }

        inline float getDeltaTime() const {
            return delta;
        }
        inline float getTotalTime() const {
            return total;
        }

        inline void setTimeScale(float scale_) {
            scale = scale_;
        }
    };

}