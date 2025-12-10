#include "glint/core/systems/CameraSystem.h"

namespace glint::engine::core {
    /*namespace {
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
    }*/

    void CameraSystem::update(scene::CameraComponent& camera, float deltaTime) noexcept {

        JPH::Vec3 local = camera.movement;
        if (local.LengthSq() < 0.0001f) {
            return;
        }

        local = local.Normalized();

        // clang-format off
        JPH::Quat orientation = JPH::Quat::sRotation(JPH::Vec3::sAxisY(), camera.yaw) 
                            * JPH::Quat::sRotation(JPH::Vec3::sAxisX(), camera.pitch)
                            * JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), camera.roll);
        // clang-format on

        camera.position += orientation * local * (camera.speed * deltaTime);
        camera.isViewDirty = true;
    }

    void CameraSystem::rotate(scene::CameraComponent& camera, float dx, float dy) noexcept {
        static constexpr float HALF_PI_RAD = JPH::DegreesToRadians(90.0f);

        camera.yaw += dx * camera.sensitivity;
        camera.pitch += dy * camera.sensitivity;
        camera.pitch = JPH::Clamp(camera.pitch, -HALF_PI_RAD + 0.001f, HALF_PI_RAD - 0.001f);

        camera.isViewDirty = true;
    }

    JPH::Mat44 CameraSystem::viewMatrix(const scene::CameraComponent& camera) const {
        if (camera.isViewDirty) {
            // clang-format off
            JPH::Quat orientation = JPH::Quat::sRotation(JPH::Vec3::sAxisY(), camera.yaw) 
                               * JPH::Quat::sRotation(JPH::Vec3::sAxisX(), camera.pitch)
                               * JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), camera.roll);
            // clang-format on

            JPH::Vec3 forward = orientation * JPH::Vec3(0.0f, 0.0f, -1.0f);
            JPH::Vec3 up = orientation * JPH::Vec3(0.0f, 1.0f, 0.0f);

            camera.view = JPH::Mat44::sLookAt(camera.position, camera.position + forward, up);
            camera.isViewDirty = false;
        }

        return camera.view;
    }

    JPH::Mat44 CameraSystem::projectionMatrix(const scene::CameraComponent& camera) const {
        if (camera.isProjectionDirty) {
            camera.projection = JPH::Mat44::sPerspective(camera.fov, camera.aspectRatio, camera.nearPlane, camera.farPlane);
            camera.isProjectionDirty = false;
        }

        return camera.projection;
    }

    JPH::Mat44 CameraSystem::viewProjectionMatrix(const scene::CameraComponent& camera) const {
        return projectionMatrix(camera) * viewMatrix(camera);
    }
}