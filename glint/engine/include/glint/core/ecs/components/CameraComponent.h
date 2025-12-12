#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <Jolt/Math/Vec3.h>

namespace glint::engine::core {

    struct CameraComponent {
        // --- projection ---
        float fov = 45.0f;
        float aspectRatio = 16.0f / 9.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        // --- movement ---
        JPH::Vec3 movement = JPH::Vec3::sZero();
        float speed = 2.0f;

        // --- orientation ---
        JPH::Vec3 position = JPH::Vec3::sZero();
        float yaw = 0.0f;
        float pitch = 0.0f;
        float roll = 0.0f;
        float sensitivity = 0.002f;

        // --- states ---
        mutable bool isViewDirty = true;
        mutable bool isProjectionDirty = true;

        // --- cache ---
        mutable JPH::Mat44 view = JPH::Mat44::sIdentity();
        mutable JPH::Mat44 projection = JPH::Mat44::sIdentity();
    };

}