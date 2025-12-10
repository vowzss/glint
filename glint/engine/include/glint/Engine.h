#pragma once

#include <memory>

namespace glint::engine {
    namespace core {
        struct TimeManager;
        struct InputManager;
        struct AssetManager;
        struct CameraSystem;
    }
    namespace graphics {
        struct Window;
        struct Renderer;
    }
    namespace scene {
        struct World;
    }
}

namespace glint {

    struct Engine {
      private:
        // --- core systems ---
        std::unique_ptr<engine::core::TimeManager> time = nullptr;
        std::unique_ptr<engine::core::InputManager> inputs = nullptr;
        std::unique_ptr<engine::core::AssetManager> assets = nullptr;

        // --- graphics systems ---
        std::unique_ptr<engine::graphics::Window> window = nullptr;
        std::unique_ptr<engine::graphics::Renderer> renderer = nullptr;

        // --- scene ---
        std::unique_ptr<engine::scene::World> world = nullptr;
        std::unique_ptr<engine::core::CameraSystem> cameras = nullptr;

      public:
        Engine() = delete;
        Engine(int width, int height);
        ~Engine();

      public:
        void run();
    };
}