#pragma once

struct GLFWwindow;

namespace glint::engine {
    namespace core {
        class InputManager;
    }
}

namespace glint::engine::graphics {

    class Window {
        int m_width, m_height;

        GLFWwindow* m_handle;
        core::InputManager* m_inputs;

      public:
        Window() = delete;
        Window(int width, int height, const char* title, core::InputManager* inputs);

        ~Window();

        // --- methods ---
        void present() const;
        void poll() const;

        // --- getters ---
        inline const int& width() const noexcept {
            return m_width;
        }

        inline const int& height() const noexcept {
            return m_height;
        }

        bool isRunning() const noexcept;

        inline GLFWwindow* raw() const noexcept {
            return m_handle;
        }
    };

}