#pragma once

struct GLFWwindow;

namespace glint::engine {
    namespace core {
        struct InputManager;
    }
}

namespace glint::engine::graphics {

    class Window {
        int m_width, m_height;

        GLFWwindow* m_handle;
        core::InputManager* m_inputs;

      public:
        Window(int width, int height, const char* title, core::InputManager* inputs);
        ~Window();

        void present() const;
        void poll() const;

        int getWidth() const;
        int getHeight() const;
        bool isRunning() const;

        inline GLFWwindow* raw() const noexcept {
            return m_handle;
        }
    };

}