#pragma once

#include <memory>

namespace glint::engine {
    namespace core {
        class TimeManager;
        class InputManager;
        class AssetManager;
        class CameraSystem;
        class World;
    }
    namespace graphics {
        class Window;
        class Renderer;
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
        std::unique_ptr<engine::core::World> world;
        std::unique_ptr<engine::core::CameraSystem> cameras;

      public:
        Engine() = delete;
        Engine(int width, int height);

        ~Engine();

        void run();
    };
}