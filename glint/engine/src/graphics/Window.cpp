#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glint/core/InputManager.h"
#include "glint/graphics/Window.h"

namespace glint::engine {
    using namespace core;

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
                    return InputAction::Unknown;
            }
        }
    }

    namespace graphics {
        Window::Window(int width_, int height_, const char* title_, core::InputManager* inputManager_)
            : width(width_), height(height_), inputManager(inputManager_) {
            if (!glfwInit()) {
                throw std::runtime_error("GLFW | failed to initialize!");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            handle = glfwCreateWindow(width, height, title_, nullptr, nullptr);

            if (!handle) {
                throw std::runtime_error("GLFW | failed to create window!");
            }

            glfwSetWindowUserPointer(handle, this);

            glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* w, int width, int height) {

            });

            glfwSetKeyCallback(handle, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
                Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
                self->inputManager->dispatch(InputEvent::key(key, translateKeyAction(action)));
            });

            glfwSetMouseButtonCallback(handle, [](GLFWwindow* w, int button, int action, int mods) {
                Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
                self->inputManager->dispatch(InputEvent::button(button, translateKeyAction(action)));
            });

            glfwSetCursorPosCallback(handle, [](GLFWwindow* w, double x, double y) {
                Window* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
                self->inputManager->dispatch(InputEvent::cursor(x, y));
            });

            inputManager_->subscribe(GLFW_KEY_ESCAPE, InputAction::Pressed,
                [&](int key, InputAction action) { glfwSetWindowShouldClose(handle, GLFW_TRUE); });
        }

        Window::~Window() {
            if (handle) {
                glfwDestroyWindow(handle);
            }

            glfwTerminate();
        }

        void Window::present() const {
            glfwSwapBuffers(handle);
        }

        void Window::pollEvents() const {
            glfwPollEvents();

            inputManager->poll();
        }

        int Window::getWidth() const {
            int w, h;
            glfwGetFramebufferSize(handle, &w, &h);
            return w;
        }

        int Window::getHeight() const {
            int w, h;
            glfwGetFramebufferSize(handle, &w, &h);
            return h;
        }

        bool Window::isRunning() const {
            return !glfwWindowShouldClose(handle);
        }
    }
}