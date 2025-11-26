#pragma once

#include <functional>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Window {
    using ResizeCallback = std::function<void(int width, int height)>;
    using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
    using CloseCallback = std::function<void()>;

  private:
    GLFWwindow* m_window = nullptr;

    KeyCallback onKey;
    CloseCallback onClose;
    ResizeCallback onResize;

  public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;

    void setResizeCallback(ResizeCallback cb);
    void setKeyCallback(KeyCallback cb);
    void setCloseCallback(CloseCallback cb);

    int getWidth() const;
    int getHeight() const;

    inline GLFWwindow* raw() const { return m_window; }

  private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void windowCloseCallback(GLFWwindow* window);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};