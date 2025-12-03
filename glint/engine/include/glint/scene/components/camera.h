#pragma once

#include "Transform.h"

namespace glint::engine::scene {
    namespace components {
        struct Camera {
          private:
            float fov = 45.0f;
            float aspectRatio = 16.0f / 9.0f;
            float nearPlane = 0.1f;
            float farPlane = 100.0f;

            Transform transform = {};
            float speed = 5.0f;

            float yaw = 0.0f;
            float pitch = 0.0f;
            float sensitivity = 0.002f;

            mutable JPH::Mat44 view;
            mutable bool isViewDirty = true;

            mutable JPH::Mat44 projection;
            mutable bool isProjectionDirty = true;

          public:
            Camera() { transform.position = JPH::Vec3{0.0f, 0.0f, 2.0f}; }

            inline void setFov(float value) {
                fov = value;
                isProjectionDirty = true;
            }

            inline void setAspectRatio(float value) {
                aspectRatio = value;
                isProjectionDirty = true;
            }

            inline void setNearPlane(float value) {
                nearPlane = value;
                isProjectionDirty = true;
            }

            inline void setFarPlane(float value) {
                farPlane = value;
                isProjectionDirty = true;
            }

            inline void setPosition(const JPH::Vec3& pos) {
                transform.position = pos;
                isViewDirty = true;
            }

            inline void setRotation(const JPH::Quat& rot) {
                transform.rotation = rot;
                isViewDirty = true;
            }

            const JPH::Mat44& getViewMatrix() const;
            const JPH::Mat44& getProjectionMatrix() const;

            inline void forward(float amount) {
                JPH::Vec3 forward = transform.rotation * JPH::Vec3(0.0f, 0.0f, -1.0f);
                transform.position += forward * amount * speed;
                isViewDirty = true;
            }

            inline void backward(float amount) {
                JPH::Vec3 forward = transform.rotation * JPH::Vec3(0.0f, 0.0f, -1.0f);
                transform.position -= forward * amount * speed;
                isViewDirty = true;
            }

            inline void right(float amount) {
                JPH::Vec3 right = transform.rotation * JPH::Vec3(1.0f, 0.0f, 0.0f);
                transform.position += right * amount * speed;
                isViewDirty = true;
            }

            inline void left(float amount) {
                JPH::Vec3 right = transform.rotation * JPH::Vec3(1.0f, 0.0f, 0.0f);
                transform.position -= right * amount * speed;
                isViewDirty = true;
            }

            inline void up(float amount) {
                JPH::Vec3 up = transform.rotation * JPH::Vec3(0.0f, 1.0f, 0.0f);
                transform.position += up * amount * speed;
                isViewDirty = true;
            }

            inline void down(float amount) {
                JPH::Vec3 up = transform.rotation * JPH::Vec3(0.0f, 1.0f, 0.0f);
                transform.position -= up * amount * speed;
                isViewDirty = true;
            }

            void rotate(float x, float y);
        };
    }
}