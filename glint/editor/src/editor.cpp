#include "glint/editor.h"

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glint/graphics/renderer.h"

namespace glint {
    editor::editor(const resolution_info& res) : res_(res) {
        window = std::make_unique<Window>(res.width, res.height, "Vulkan");

        createRenderer();

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer->getInstance(), window->raw(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer->init(surface);
    }

    editor::~editor() {
        delete renderer;
        renderer = nullptr;

        window.reset();
        glfwTerminate();
    }

    void editor::run() {
        while (!window->shouldClose()) {
            renderer->beginFrame();
            renderer->recordFrame();
            renderer->endFrame();

            window->swapBuffers();
            window->pollEvents();
        }
    }

    void editor::createRenderer() {
        uint32_t extensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);

        if (validationLayersEnabled) {
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        renderer = new glint::engine::graphics::renderer(res_.width, res_.height, extensions);
    }
}