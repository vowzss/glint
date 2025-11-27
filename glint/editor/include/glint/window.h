#pragma once

#include <functional>
#include <string>

struct GLFWwindow;

namespace glint {
    struct Window {
      private:
        using ResizeCallback = std::function<void(int width, int height)>;
        using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
        using CloseCallback = std::function<void()>;

      private:
        GLFWwindow* window = nullptr;

        KeyCallback onKey;
        CloseCallback onClose;
        ResizeCallback onResize;

      public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void present() const;
        void processEvents() const;

        void onResizeCallback(ResizeCallback cb);
        void onKeyCallback(KeyCallback cb);
        void onCloseCallback(CloseCallback cb);

        int getWidth() const;
        int getHeight() const;
        bool isRunning() const;

        inline GLFWwindow* raw() const { return window; }

      private:
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void windowCloseCallback(GLFWwindow* window);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    };
}