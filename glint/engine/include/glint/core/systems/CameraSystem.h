#pragma once

#include <cassert>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>

#include "glint/scene/components/CameraComponent.h"

namespace glint::engine::core {

    struct CameraSnapshot {
        JPH::Mat44 view;
        JPH::Mat44 proj;
        JPH::Mat44 viewProj;

      public:
        // --- getters ---
        inline const void* data() const noexcept {
            return static_cast<const void*>(&view);
        }

        static inline size_t size() noexcept {
            return sizeof(JPH::Mat44) * 3;
        }
    };

    class CameraSystem {
        std::unique_ptr<scene::CameraComponent> m_default;
        scene::CameraComponent* m_active = nullptr;

      public:
        CameraSystem() {
            m_default = std::make_unique<scene::CameraComponent>();
            m_default->position = {0.0f, 0.0f, 2.0f};

            m_active = m_default.get();
        }

        void update(scene::CameraComponent& camera, float deltaTime) noexcept;
        void rotate(scene::CameraComponent& camera, float dx, float dy) noexcept;

        inline void move(scene::CameraComponent& camera, const JPH::Vec3& v) noexcept {
            // clang-format off
            camera.movement = JPH::Vec3{
                std::clamp(v.GetX(), -1.0f, 1.0f), 
                std::clamp(v.GetY(), -1.0f, 1.0f), 
                std::clamp(v.GetZ(), -1.0f, 1.0f)
            };
            // clang-format on
        }

        inline void forward(scene::CameraComponent& camera, float v) noexcept {
            JPH::Vec3 movement = camera.movement;
            movement.SetX(v);
            move(camera, movement);
        }

        inline void right(scene::CameraComponent& camera, float v) noexcept {
            JPH::Vec3 movement = camera.movement;
            movement.SetY(v);
            move(camera, movement);
        }

        inline void up(scene::CameraComponent& camera, float v) noexcept {
            JPH::Vec3 movement = camera.movement;
            movement.SetZ(v);
            move(camera, movement);
        }

        inline void activate(scene::CameraComponent& camera) noexcept {
            m_active = &camera;
        }

        // --- setters ---
        inline void fov(scene::CameraComponent& camera, float value) noexcept {
            camera.fov = std::clamp(value, 1.0f, 179.0f);
            camera.isProjectionDirty = true;
        }

        inline void aspectRatio(scene::CameraComponent& camera, float value) noexcept {
            camera.aspectRatio = std::max(0.01f, value);
            camera.isProjectionDirty = true;
        }

        inline void nearPlane(scene::CameraComponent& camera, float value) noexcept {
            camera.nearPlane = std::max(0.001f, value);
            camera.isProjectionDirty = true;
        }

        inline void farPlane(scene::CameraComponent& camera, float value) noexcept {
            camera.farPlane = std::max(camera.nearPlane + 0.001f, value);
            camera.isProjectionDirty = true;
        }

        // --- getters ---
        JPH::Mat44 viewMatrix(const scene::CameraComponent& camera) const;
        JPH::Mat44 projectionMatrix(const scene::CameraComponent& camera) const;
        JPH::Mat44 viewProjectionMatrix(const scene::CameraComponent& camera) const;

        inline scene::CameraComponent& active() {
            return *m_active;
        }

        inline const scene::CameraComponent& active() const {
            return *m_active;
        }

        inline CameraSnapshot snapshot(const scene::CameraComponent& camera) const noexcept {
            const auto proj = projectionMatrix(camera);
            const auto view = viewMatrix(camera);
            return CameraSnapshot{view, proj, proj * view};
        }

        inline CameraSnapshot snapshot() const noexcept {
            return snapshot(*m_active);
        }
    };

}