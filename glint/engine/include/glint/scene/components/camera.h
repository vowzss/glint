#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <Jolt/Math/Vec3.h>

namespace glint::engine::scene::components {
    struct camera {
        JPH::Vec3 position{0.0f, 0.0f, 2.0f};
        JPH::Vec3 front{0.0f, 0.0f, -1.0f};
        JPH::Vec3 up{0.0f, 1.0f, 0.0f};

        float fov = 45.0f;
        float aspect = 16.0f / 9.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

      public:
        camera() = default;
        camera(JPH::Vec3 pos, JPH::Vec3 front, JPH::Vec3 up, float fov = 45.0f, float aspect = 16.0f / 9.0f, float near = 0.1f, float far = 100.0f)
            : position(pos), front(front), up(up), fov(fov), aspect(aspect), nearPlane(near), farPlane(far) {}

        JPH::Mat44 getViewMatrix() const;
        JPH::Mat44 getProjectionMatrix() const;

      private:
        JPH::Mat44 lookAt(const JPH::Vec3& eye, const JPH::Vec3& center, const JPH::Vec3& up) const;
    };
}