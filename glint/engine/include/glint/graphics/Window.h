#pragma once

struct GLFWwindow;

namespace glint::engine {
    namespace core {
        struct InputManager;
    }
}

namespace glint::engine::graphics {

    struct Window {
      private:
        int width, height;

        GLFWwindow* handle = nullptr;
        core::InputManager* inputManager;

      public:
        Window(int width_, int height_, const char* title_, core::InputManager* inputManager_);
        ~Window();

        void present() const;
        void poll() const;

        int getWidth() const;
        int getHeight() const;
        bool isRunning() const;

        inline GLFWwindow* raw() const { return handle; }
    };

}