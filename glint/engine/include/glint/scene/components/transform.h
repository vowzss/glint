#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Math/Vec3.h>

namespace glint::engine::scene {
    namespace components {
        struct Transform {
          public:
            JPH::Vec3 position = JPH::Vec3::sZero();
            JPH::Quat rotation = JPH::Quat::sIdentity();
            JPH::Vec3 scale = JPH::Vec3::sOne();

          public:
            Transform() = default;

            inline void translateBy(const JPH::Vec3& delta) { position += delta; }
            inline void rotateBy(const JPH::Quat& delta) { rotation = delta * rotation; }
            inline void scaleBy(const JPH::Vec3& factor) { scale *= factor; }

            inline JPH::Mat44 toMatrix() const {
                const JPH::Mat44 r = JPH::Mat44::sRotationTranslation(rotation, position);
                const JPH::Mat44 s = JPH::Mat44::sScale(scale);

                return r * s;
            }
        };
    }
}