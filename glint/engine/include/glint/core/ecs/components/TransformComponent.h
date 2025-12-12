#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Math/Vec3.h>

namespace glint::engine::core {

    class TransformComponent {
        JPH::Vec3 pos = JPH::Vec3::sZero();
        JPH::Quat rot = JPH::Quat::sIdentity();
        JPH::Vec3 scl = JPH::Vec3::sOne();

        // --- cache ---
        mutable JPH::Mat44 mat = JPH::Mat44::sIdentity();
        mutable bool isDirty = true;

      public:
        TransformComponent() noexcept = default;
        ~TransformComponent() noexcept = default;

        TransformComponent(const TransformComponent&) = delete;
        TransformComponent& operator=(const TransformComponent&) = delete;

        TransformComponent(TransformComponent&&) noexcept = default;
        TransformComponent& operator=(TransformComponent&&) = default;

        // --- utils ---
        inline void translateBy(const JPH::Vec3& delta) {
            pos += delta;
            isDirty = true;
        }

        inline void rotateBy(const JPH::Quat& delta) {
            rot = delta * rot;
            isDirty = true;
        }

        inline void scaleBy(const JPH::Vec3& factor) {
            scl *= factor;
            isDirty = true;
        }

        // --- setters ---
        inline void setPosition(const JPH::Vec3& value) {
            pos = value;
            isDirty = true;
        }

        inline void setRotation(const JPH::Quat& value) {
            rot = value;
            isDirty = true;
        }

        inline void setScale(const JPH::Vec3& value) {
            scl = value;
            isDirty = true;
        }

        // --- getters ---
        inline const JPH::Vec3& position() const {
            return pos;
        }

        inline const JPH::Quat& rotation() const {
            return rot;
        }

        inline const JPH::Vec3& scale() const {
            return scl;
        }

        inline const JPH::Mat44& matrix() const noexcept {
            if (isDirty) {
                mat = JPH::Mat44::sScale(scl) * JPH::Mat44::sRotationTranslation(rot, pos);
                isDirty = false;
            }

            return mat;
        }
    };

}