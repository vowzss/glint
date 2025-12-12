#pragma once

#include "glint/core/ecs/components/CameraComponent.h"

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
        std::unique_ptr<CameraComponent> m_default = nullptr;
        CameraComponent* m_active = nullptr;

      public:
        CameraSystem() {
            m_default = std::make_unique<CameraComponent>();
            m_default->position = {0.0f, 0.0f, 2.0f};

            m_active = m_default.get();
        }

        ~CameraSystem() noexcept = default;

        void update(CameraComponent& camera, float deltaTime) noexcept;
        void rotate(CameraComponent& camera, float dx, float dy) noexcept;

        inline void move(CameraComponent& camera, const JPH::Vec3& v) noexcept {
            // clang-format off
            camera.movement = JPH::Vec3{
                std::clamp(v.GetX(), -1.0f, 1.0f), 
                std::clamp(v.GetY(), -1.0f, 1.0f), 
                std::clamp(v.GetZ(), -1.0f, 1.0f)
            };
            // clang-format on
        }

        inline void forward(CameraComponent& camera, float v) noexcept {
            JPH::Vec3 movement = camera.movement;
            movement.SetX(v);
            move(camera, movement);
        }

        inline void right(CameraComponent& camera, float v) noexcept {
            JPH::Vec3 movement = camera.movement;
            movement.SetY(v);
            move(camera, movement);
        }

        inline void up(CameraComponent& camera, float v) noexcept {
            JPH::Vec3 movement = camera.movement;
            movement.SetZ(v);
            move(camera, movement);
        }

        inline void activate(CameraComponent& camera) noexcept {
            m_active = &camera;
        }

        // --- setters ---
        inline void fov(CameraComponent& camera, float value) noexcept {
            camera.fov = std::clamp(value, 1.0f, 179.0f);
            camera.isProjectionDirty = true;
        }

        inline void aspectRatio(CameraComponent& camera, float value) noexcept {
            camera.aspectRatio = std::max(0.01f, value);
            camera.isProjectionDirty = true;
        }

        inline void nearPlane(CameraComponent& camera, float value) noexcept {
            camera.nearPlane = std::max(0.001f, value);
            camera.isProjectionDirty = true;
        }

        inline void farPlane(CameraComponent& camera, float value) noexcept {
            camera.farPlane = std::max(camera.nearPlane + 0.001f, value);
            camera.isProjectionDirty = true;
        }

        // --- getters ---
        JPH::Mat44 viewMatrix(const CameraComponent& camera) const;
        JPH::Mat44 projectionMatrix(const CameraComponent& camera) const;
        JPH::Mat44 viewProjectionMatrix(const CameraComponent& camera) const;

        inline CameraComponent& active() noexcept {
            return *m_active;
        }

        inline const CameraComponent& active() const noexcept {
            return *m_active;
        }

        inline CameraSnapshot snapshot(const CameraComponent& camera) const noexcept {
            const auto proj = projectionMatrix(camera);
            const auto view = viewMatrix(camera);
            return CameraSnapshot{view, proj, proj * view};
        }

        inline CameraSnapshot snapshot() const noexcept {
            return snapshot(*m_active);
        }
    };

}