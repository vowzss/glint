#pragma once

#include "Transform.h"

namespace glint::engine::scene {

    namespace components {
        struct Camera {
          private:
            float fov = 45.0f;
            float aspectRatio = 16.0f / 9.0f;
            float near = 0.1f;
            float far = 100.0f;

            Transform transform = {};

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

            inline const JPH::Mat44& getViewMatrix() const {
                if (isViewDirty) {
                    JPH::Vec3 forward = transform.rotation * JPH::Vec3(0.0f, 0.0f, -1.0f);
                    JPH::Vec3 up = transform.rotation * JPH::Vec3(0.0f, 1.0f, 0.0f);
                    view = JPH::Mat44::sLookAt(transform.position, transform.position + forward, up);
                    isViewDirty = false;
                }

                return view;
            }

            // todo: move impl here (find computing alternative)
            const JPH::Mat44& getProjectionMatrix() const;
        };
    }
}