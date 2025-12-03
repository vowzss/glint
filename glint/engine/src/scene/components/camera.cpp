#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec4.h>

#include "glint/scene/components/Camera.h"

namespace glint::engine::scene {
    namespace components {
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

        const JPH::Mat44& Camera::getViewMatrix() const {
            if (isViewDirty) {
                JPH::Vec3 forward = transform.rotation * JPH::Vec3(0.0f, 0.0f, -1.0f);
                JPH::Vec3 up = transform.rotation * JPH::Vec3(0.0f, 1.0f, 0.0f);
                view = JPH::Mat44::sLookAt(transform.position, transform.position + forward, up);
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

        void Camera::rotate(float x, float y) {
            yaw -= x * sensitivity;
            pitch -= y * sensitivity;

            pitch = std::clamp(pitch, -1.57f, 1.57f);

            JPH::Quat yawRot = JPH::Quat::sRotation(JPH::Vec3(0, 1, 0), yaw);
            JPH::Quat pitchRot = JPH::Quat::sRotation(JPH::Vec3(1, 0, 0), pitch);

            setRotation(yawRot * pitchRot);
        }
    }
}