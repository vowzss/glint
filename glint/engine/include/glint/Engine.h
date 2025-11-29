#pragma once

#include <memory>
#include <vector>

namespace glint {
    namespace engine::core {
        struct InputManager;
    }

    namespace engine::graphics {
        struct Window;
        struct Renderer;
    }

    struct Engine {
      private:
        std::unique_ptr<engine::graphics::Window> window = nullptr;
        std::unique_ptr<engine::core::InputManager> inputManager = nullptr;

        std::unique_ptr<engine::graphics::Renderer> renderer = nullptr;

      public:
        Engine() = delete;
        Engine(int width, int height);

        ~Engine();

        void run();

      private:
        std::vector<const char*> getRequiredExtensions() const;
    };
}