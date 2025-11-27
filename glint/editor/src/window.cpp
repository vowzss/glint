#include "glint/Window.h"

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace glint {
    Window::Window(int width, int height, const std::string& title) {
        if (!glfwInit()) {
            throw std::runtime_error("GLFW | failed to initialize!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!window) {
            throw std::runtime_error("GLFW | failed to create window!");
        }

        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetWindowCloseCallback(window, windowCloseCallback);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        glfwMakeContextCurrent(window);

        onKeyCallback([&](int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        });
    }

    Window::~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }

        glfwTerminate();
    }

    void Window::present() const {
        glfwSwapBuffers(window);
    }

    void Window::processEvents() const {
        glfwPollEvents();
    }

    void Window::onKeyCallback(KeyCallback cb) {
        onKey = std::move(cb);
    }

    void Window::onCloseCallback(CloseCallback cb) {
        onClose = std::move(cb);
    }

    void Window::onResizeCallback(ResizeCallback cb) {
        onResize = std::move(cb);
    }

    int Window::getWidth() const {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        return w;
    }

    int Window::getHeight() const {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        return h;
    }

    bool Window::isRunning() const {
        return !glfwWindowShouldClose(window);
    }

    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win && win->onKey) {
            win->onKey(key, scancode, action, mods);
        }
    }

    void Window::windowCloseCallback(GLFWwindow* window) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win && win->onClose) {
            win->onClose();
        }
    }

    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win && win->onResize) {
            win->onResize(width, height);
        }
    }
}