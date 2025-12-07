#pragma once

#include "Transform.h"

namespace glint::engine::scene::components {

    struct CameraMovement {
        float forward = 0.0f;
        float right = 0.0f;
        float up = 0.0f;
    };

    struct CameraSnapshot {
        JPH::Mat44 view;
        JPH::Mat44 proj;
        JPH::Mat44 viewProj;

        inline const void* data() const noexcept { return static_cast<const void*>(&view); }
        inline size_t size() const noexcept { return sizeof(JPH::Mat44) * 3; }
    };

    class Camera {
      private:
        float fov = 45.0f;
        float aspectRatio = 16.0f / 9.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        CameraMovement movement;

        Transform transform = {};
        float speed = 2.0f;

        float yaw = 0.0f;
        float pitch = 0.0f;
        float sensitivity = 0.002f;

        mutable JPH::Mat44 view;
        mutable bool isViewDirty = true;

        mutable JPH::Mat44 projection;
        mutable bool isProjectionDirty = true;

      public:
        Camera() {
            transform.onChanged = [this]() { isViewDirty = true; };
            transform.setPosition(JPH::Vec3{0.0f, 0.0f, 2.0f});
            transform.setRotation(JPH::Quat::sIdentity());
        }

        inline void setFov(float value) noexcept {
            fov = value;
            isProjectionDirty = true;
        }

        inline void setAspectRatio(float value) noexcept {
            aspectRatio = value;
            isProjectionDirty = true;
        }

        inline void setNearPlane(float value) noexcept {
            nearPlane = value;
            isProjectionDirty = true;
        }

        inline void setFarPlane(float value) noexcept {
            farPlane = value;
            isProjectionDirty = true;
        }

        inline void forward(float v) noexcept { movement.forward = v; }
        inline void right(float v) noexcept { movement.right = v; }
        inline void up(float v) noexcept { movement.up = v; }

        void rotate(float dx, float dy) noexcept;

        inline void update(float deltaTime) noexcept {
            JPH::Vec3 local(movement.right, movement.up, -movement.forward);
            if (local.LengthSq() < 0.0001f) return;

            local = local.Normalized();
            JPH::Vec3 direction = transform.getRotation() * local;
            transform.translateBy(direction * speed * deltaTime);
        }

        const JPH::Mat44& getViewMatrix() const;
        const JPH::Mat44& getProjectionMatrix() const;
        JPH::Mat44 getViewProjectionMatrix() const;

        inline CameraSnapshot getSnapshot() const noexcept { return CameraSnapshot{view, projection, projection * view}; }
    };
}