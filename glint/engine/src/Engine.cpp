#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glint/Engine.h"
#include "glint/core/InputManager.h"
#include "glint/core/Logger.h"
#include "glint/graphics/Renderer.h"
#include "glint/graphics/Window.h"

namespace glint {
    using namespace engine::graphics;
    using namespace engine::core;

    Engine::Engine(int width, int height) {
        Logger::init();

        inputManager = std::make_unique<InputManager>();
        window = std::make_unique<Window>(width, height, "Vulkan", inputManager.get());

        renderer = std::make_unique<Renderer>(width, height, getRequiredExtensions());

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer->getInstance(), window->raw(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer->init(std::move(surface));
    }

    Engine::~Engine() {
        inputManager.reset();
        renderer.reset();
        window.reset();
    }

    void Engine::run() {
        LOG_INFO("starting main loop");

        using clock = std::chrono::high_resolution_clock;
        auto lastTime = clock::now();

        while (window->isRunning()) {
            auto now = clock::now();
            float dt = std::chrono::duration<float>(now - lastTime).count();
            lastTime = now;

            window->pollEvents();
            inputManager->poll();

            renderer->beginFrame();
            renderer->recordFrame();
            renderer->endFrame();

            window->present();
        }

        LOG_INFO("exiting main loop");
    }

    std::vector<const char*> Engine::getRequiredExtensions() const {
        uint32_t count;
        const char** ext = glfwGetRequiredInstanceExtensions(&count);

        std::vector<const char*> extensions(ext, ext + count);

        for (uint32_t i = 0; i < count; ++i) {
            std::cout << extensions[i] << "\n";
        }

        return extensions;
    }

}