#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Math/Vec3.h>

namespace glint::engine::scene {
    namespace components {
        struct Transform {
            std::function<void()> onChanged;

          private:
            JPH::Vec3 position = JPH::Vec3::sZero();
            JPH::Quat rotation = JPH::Quat::sIdentity();
            JPH::Vec3 scale = JPH::Vec3::sOne();

          public:
            Transform() = default;

            inline void setPosition(const JPH::Vec3& value) {
                position = value;
                if (onChanged) onChanged();
            }

            inline void setRotation(const JPH::Quat& value) {
                rotation = value;
                if (onChanged) onChanged();
            }

            inline void setScale(const JPH::Vec3& value) {
                scale = value;
                if (onChanged) onChanged();
            }

            inline void translateBy(const JPH::Vec3& delta) {
                position += delta;
                if (onChanged) onChanged();
            }

            inline void rotateBy(const JPH::Quat& delta) {
                rotation = delta * rotation;
                if (onChanged) onChanged();
            }

            inline void scaleBy(const JPH::Vec3& factor) {
                scale *= factor;
                if (onChanged) onChanged();
            }

            inline JPH::Mat44 toMatrix() const {
                const JPH::Mat44 r = JPH::Mat44::sRotationTranslation(rotation, position);
                const JPH::Mat44 s = JPH::Mat44::sScale(scale);

                return s * r;
            }

            inline const JPH::Vec3& getPosition() const { return position; }
            inline const JPH::Quat& getRotation() const { return rotation; }
            inline const JPH::Vec3& getScale() const { return scale; }
        };
    }
}