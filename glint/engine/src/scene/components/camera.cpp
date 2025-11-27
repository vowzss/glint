#include "glint/scene/components/Camera.h"

#include <Jolt/Math/Vec4.h>

namespace glint::engine::scene::components {
    namespace {
        JPH::Mat44 perspective(float fov, float aspect, float near, float far) {
            float f = 1.0f / tan(fov * 0.5f * 3.14159265f / 180.0f);

            // clang-format off
            return JPH::Mat44(
                JPH::Vec4(f / aspect, 0, 0, 0),
                JPH::Vec4(0, -f, 0,  0),
                JPH::Vec4(0,  0, far / (near - far), -1),
                JPH::Vec4(0,  0, (near * far) / (near - far), 0)
            );
            // clang-format on
        }

        JPH::Mat44 orthographic(float left, float right, float bottom, float top, float near, float far) {
            // clang-format off
             return JPH::Mat44(
                JPH::Vec4(2.0f / (right - left), 0.0f, 0.0f, 0.0f),
                JPH::Vec4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f),
                JPH::Vec4(0.0f, 0.0f, 1.0f / (far - near), 0.0f),
                JPH::Vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -near / (far - near), 1.0f)
            );
            // clang-format on
        }

    }

    const JPH::Mat44& Camera::getProjectionMatrix() const {
        if (isProjectionDirty) {
            projection = perspective(fov, aspectRatio, near, far);
            isProjectionDirty = false;
        }

        return projection;
    }
}