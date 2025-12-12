#include "glint/core/systems/CameraSystem.h"

namespace glint::engine::core {

    void CameraSystem::update(CameraComponent& camera, float deltaTime) noexcept {
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

    void CameraSystem::rotate(CameraComponent& camera, float dx, float dy) noexcept {
        static constexpr float HALF_PI_RAD = JPH::DegreesToRadians(90.0f);

        camera.yaw += dx * camera.sensitivity;
        camera.pitch += dy * camera.sensitivity;
        camera.pitch = JPH::Clamp(camera.pitch, -HALF_PI_RAD + 0.001f, HALF_PI_RAD - 0.001f);

        camera.isViewDirty = true;
    }

    JPH::Mat44 CameraSystem::viewMatrix(const CameraComponent& camera) const {
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

    JPH::Mat44 CameraSystem::projectionMatrix(const CameraComponent& camera) const {
        if (camera.isProjectionDirty) {
            camera.projection = JPH::Mat44::sPerspective(camera.fov, camera.aspectRatio, camera.nearPlane, camera.farPlane);
            camera.isProjectionDirty = false;
        }

        return camera.projection;
    }

    JPH::Mat44 CameraSystem::viewProjectionMatrix(const CameraComponent& camera) const {
        return projectionMatrix(camera) * viewMatrix(camera);
    }
}