#pragma once

#include <memory>
#include <vector>

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
        class RenderLayer;
    }
}

namespace glint {

    class Engine {
        // --- core systems ---
        std::unique_ptr<engine::core::TimeManager> time;
        std::unique_ptr<engine::core::InputManager> inputs;
        std::unique_ptr<engine::core::AssetManager> assets;

        // --- scene ---
        std::unique_ptr<engine::core::World> world;
        std::unique_ptr<engine::core::CameraSystem> cameras;

        // ---  ---
        std::vector<std::unique_ptr<engine::graphics::RenderLayer>> layers;

        // --- graphics systems ---
        std::unique_ptr<engine::graphics::Window> window;
        std::unique_ptr<engine::graphics::Renderer> renderer;

      public:
        Engine() = delete;
        Engine(int width, int height);

        ~Engine();

        void run();
    };
}