#pragma once

#include "Transform.h"

namespace glint::engine::scene {
    namespace components {
        struct CameraInput {
            float forward = 0.0f;
            float right = 0.0f;
            float up = 0.0f;
        };

        struct Camera {
          private:
            float fov = 45.0f;
            float aspectRatio = 16.0f / 9.0f;
            float nearPlane = 0.1f;
            float farPlane = 100.0f;

            CameraInput input;

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

            void rotate(float x, float y);

            inline void update(float deltaTime) {
                JPH::Vec3 direction = transform.getRotation() * JPH::Vec3(input.right, 0.0f, input.forward);

                if (direction.LengthSq() > 0.0f) {
                    direction = direction.Normalized();
                    transform.translateBy(direction * speed * deltaTime);
                }
            }

            const JPH::Mat44& getViewMatrix() const;
            const JPH::Mat44& getProjectionMatrix() const;

            CameraInput& getInput() { return input; }
        };
    }
}