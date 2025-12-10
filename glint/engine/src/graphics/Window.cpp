#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glint/core/managers/InputManager.h"
#include "glint/graphics/Window.h"

using namespace glint::engine::core;

namespace glint::engine::graphics {

    namespace {
        InputAction translateKeyAction(int action) {
            switch (action) {
                case GLFW_PRESS:
                    return InputAction::Pressed;
                case GLFW_RELEASE:
                    return InputAction::Released;
                case GLFW_REPEAT:
                    return InputAction::Held;
                default:
                    return InputAction::Undefined;
            }
        }
    }

    Window::Window(int width, int height, const char* title, InputManager* inputs) : m_width(width), m_height(height), m_inputs(inputs) {

        if (!glfwInit()) {
            throw std::runtime_error("GLFW | failed to initialize!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

        if (m_handle == nullptr) {
            throw std::runtime_error("GLFW | failed to create window!");
        }

        glfwSetWindowUserPointer(m_handle, this);
        glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* w, int width, int height) {

        });

        glfwSetKeyCallback(m_handle, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
            self->m_inputs->dispatch(InputEvent::key(key, translateKeyAction(action)));
        });

        glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* w, int button, int action, int mods) {
            Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
            self->m_inputs->dispatch(InputEvent::button(button, translateKeyAction(action)));
        });

        glfwSetCursorPosCallback(m_handle, [](GLFWwindow* w, double x, double y) {
            Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
            self->m_inputs->dispatch(InputEvent::cursor(x, y));
        });

        inputs->subscribe(InputType::Key, GLFW_KEY_ESCAPE, InputAction::Pressed,
            [&](int code, InputAction action) { glfwSetWindowShouldClose(m_handle, GLFW_TRUE); });
    }

    Window::~Window() {
        if (m_handle != nullptr) {
            glfwDestroyWindow(m_handle);
        }

        glfwTerminate();
    }

    void Window::present() const {
        glfwSwapBuffers(m_handle);
    }

    void Window::poll() const {
        glfwPollEvents();

        m_inputs->poll();
    }

    int Window::getWidth() const {
        int w, h;
        glfwGetFramebufferSize(m_handle, &w, &h);
        return w;
    }

    int Window::getHeight() const {
        int w, h;
        glfwGetFramebufferSize(m_handle, &w, &h);
        return h;
    }

    bool Window::isRunning() const {
        return !glfwWindowShouldClose(m_handle);
    }

}