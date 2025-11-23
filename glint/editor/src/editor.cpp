#include "glint/editor.h"

#include <cstdlib>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glint/graphics/renderer.h"

namespace glint {
    editor::editor(const resolution_info& res) : res_(res) {
        createWindow();
        createRenderer();

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer_->getInstance(), window_, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer_->init(surface);
    }

    editor::~editor() {
        delete renderer_;
        renderer_ = nullptr;

        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void editor::run() {
        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();

            if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                exit(0);
            }

            renderer_->draw();
        }
    }

    void editor::createWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("GLFW | failed to initialize!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window_ = glfwCreateWindow(res_.width, res_.height, "Vulkan Window", nullptr, nullptr);

        if (window_ == nullptr) {
            throw std::runtime_error("GLFW | failed to create window!");
        }
    }

    void editor::createRenderer() {
        uint32_t extensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);

        if (validationLayersEnabled) {
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        renderer_ = new glint::engine::graphics::renderer(res_.width, res_.height, extensions);
    }
}