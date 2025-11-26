#include "glint/window.h"

#include <iostream>
#include <stdexcept>

Window::Window(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW | failed to initialize!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!m_window) {
        throw std::runtime_error("GLFW | failed to create window!");
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetWindowCloseCallback(m_window, windowCloseCallback);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    glfwMakeContextCurrent(m_window);

    setKeyCallback([&](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }
    });
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(m_window);
}

void Window::setKeyCallback(KeyCallback cb) {
    onKey = std::move(cb);
}

void Window::setCloseCallback(CloseCallback cb) {
    onClose = std::move(cb);
}

void Window::setResizeCallback(ResizeCallback cb) {
    onResize = std::move(cb);
}

int Window::getWidth() const {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    return w;
}

int Window::getHeight() const {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    return h;
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