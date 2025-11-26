#include "glint/scene/components/camera.h"

#include <Jolt/Math/Vec4.h>

namespace glint::engine::scene::components {
    namespace {
        JPH::Mat44 perspective(float fov, float aspect, float near, float far) {
            float f = 1.0f / tan(fov * 0.5f * 3.14159265f / 180.0f);

            // clang-format off
            return JPH::Mat44(
                JPH::Vec4(f / aspect, 0, 0, 0),
                JPH::Vec4(0, -f, 0, 0),
                JPH::Vec4(0, 0, far / (near - far), -1),
                JPH::Vec4(0, 0, (near * far) / (near - far), 0)
            );
            // clang-format on
        }

        /*JPH::Mat44 orthographic(float left, float right, float bottom, float top, float near, float far) {
            mat4 result{};
            result[0][0] = 2.0f / (right - left);

            result[1][1] = 2.0f / (top - bottom);

            result[2][2] = 2.0f / (far - near);

            result[3][0] = -(right + left) / (right - left);
            result[3][1] = -(top + bottom) / (top - bottom);
            result[3][2] = -(far + near) / (far - near);
            result[3][3] = 1.0f;

            return result;
        }*/
    }

    JPH::Mat44 camera::lookAt(const JPH::Vec3& eye, const JPH::Vec3& center, const JPH::Vec3& up) const {
        JPH::Vec3 f = (center - eye).Normalized();
        JPH::Vec3 s = f.Cross(up).Normalized();
        JPH::Vec3 u = s.Cross(f);

        // clang-format off
        return JPH::Mat44(
            JPH::Vec4(s.GetX(), s.GetY(), s.GetZ(), 0.0f),
            JPH::Vec4(u.GetX(), u.GetY(), u.GetZ(), 0.0f),
            JPH::Vec4(-f.GetX(), -f.GetY(), -f.GetZ(), 0.0f),
            JPH::Vec4(-s.Dot(eye), -u.Dot(eye), f.Dot(eye), 1.0f)
        );
        // clang-format on
    }

    JPH::Mat44 camera::getViewMatrix() const {
        return lookAt(position, position + front, up);
    }

    JPH::Mat44 camera::getProjectionMatrix() const {
        return perspective(fov, aspect, nearPlane, farPlane);
    }
}