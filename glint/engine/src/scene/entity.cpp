#include "glint/scene/entity.h"

namespace glint::engine::scene {
    JPH::Mat44 entity::getModelMatrix() const {
        JPH::Mat44 T = JPH::Mat44::sTranslation(transform.position);
        JPH::Mat44 R = JPH::Mat44::sRotation(transform.rotation);

        // clang-format off
        JPH::Mat44 S(
            JPH::Vec4(transform.scale.GetX(), 0.0f, 0.0f, 0.0f),
            JPH::Vec4(0.0f, transform.scale.GetY(), 0.0f, 0.0f),
            JPH::Vec4(0.0f, 0.0f, transform.scale.GetZ(), 0.0f),
            JPH::Vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );
        // clang-format on

        return T * R * S;
    }
}