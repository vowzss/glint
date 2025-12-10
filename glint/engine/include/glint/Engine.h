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

    class Engine {
        // --- core systems ---
        std::unique_ptr<engine::core::TimeManager> time;
        std::unique_ptr<engine::core::InputManager> inputs;
        std::unique_ptr<engine::core::AssetManager> assets;

        // --- graphics systems ---
        std::unique_ptr<engine::graphics::Window> window;
        std::unique_ptr<engine::graphics::Renderer> renderer;

        // --- scene ---
        std::unique_ptr<engine::scene::World> world;
        std::unique_ptr<engine::core::CameraSystem> cameras;

      public:
        Engine() = delete;
        Engine(int width, int height);

        ~Engine();

        void run();
    };
}