#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec4.h>

#include "glint/scene/components/Camera.h"

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

    void Camera::rotate(float dx, float dy) noexcept {
        static constexpr float HALF_PI_RAD = JPH::DegreesToRadians(90.0f);

        yaw += dx * sensitivity;
        pitch += dy * sensitivity;
        pitch = JPH::Clamp(pitch, -HALF_PI_RAD + 0.001f, HALF_PI_RAD - 0.001f);

        // clang-format off
        transform.setRotation(
            JPH::Quat::sRotation(JPH::Vec3::sAxisY(), yaw) *
            JPH::Quat::sRotation(JPH::Vec3::sAxisX(), pitch)
        );
        // clang-format on
    }

    const JPH::Mat44& Camera::getViewMatrix() const {
        if (isViewDirty) {
            JPH::Vec3 forward = transform.getRotation() * JPH::Vec3(0.0f, 0.0f, -1.0f);
            JPH::Vec3 up = transform.getRotation() * JPH::Vec3(0.0f, 1.0f, 0.0f);
            view = JPH::Mat44::sLookAt(transform.getPosition(), transform.getPosition() + forward, up);
            isViewDirty = false;
        }

        return view;
    }

    const JPH::Mat44& Camera::getProjectionMatrix() const {
        if (isProjectionDirty) {
            projection = perspective(fov, aspectRatio, nearPlane, farPlane);
            isProjectionDirty = false;
        }

        return projection;
    }

    JPH::Mat44 Camera::getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

}