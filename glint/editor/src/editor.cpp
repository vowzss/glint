#include "glint/Editor.h"

#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace glint {
    using namespace engine::graphics;

    Editor::Editor(int width, int height) {
        window = std::make_unique<Window>(width, height, "Vulkan");

        renderer = std::make_unique<Renderer>(width, height, getRequiredExtensions());

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer->getInstance(), window->raw(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer->init(std::move(surface));
    }

    Editor::~Editor() {
        renderer.reset();
        window.reset();
    }

    void Editor::run() {
        while (window->isRunning()) {
            renderer->beginFrame();
            renderer->recordFrame();
            renderer->endFrame();

            window->present();
            window->processEvents();
        }
    }

    std::vector<const char*> Editor::getRequiredExtensions() const {
        uint32_t count;
        const char** ext = glfwGetRequiredInstanceExtensions(&count);

        std::vector<const char*> extensions(ext, ext + count);

        for (uint32_t i = 0; i < count; ++i) {
            std::cout << extensions[i] << "\n";
        }

        return extensions;
    }

}