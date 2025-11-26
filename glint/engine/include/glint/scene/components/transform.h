#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Math/Vec3.h>

namespace glint::engine::scene::components {
    struct transform {
        JPH::Vec3 position{};
        JPH::Quat rotation{};
        JPH::Vec3 scale = JPH::Vec3(1.0f, 1.0f, 1.0f);

      public:
        transform() = default;
        transform(const JPH::Vec3& pos, const JPH::Quat& rot, const JPH::Vec3& scl = JPH::Vec3(1.0f, 1.0f, 1.0f))
            : position(pos), rotation(rot), scale(scl) {}
    };
}