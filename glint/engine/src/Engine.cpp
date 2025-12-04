#include <stdexcept>

#include "glint/Engine.h"
#include "glint/core/InputManager.h"
#include "glint/core/Logger.h"
#include "glint/core/TimeManager.h"
#include "glint/graphics/Renderer.h"
#include "glint/graphics/Window.h"
#include "glint/scene/components/Camera.h"
#include "glint/utils/StringUtils.h"

// todo: cleanup
#include <GLFW/glfw3.h>

namespace glint {
    Engine::Engine(int width, int height) {
        using namespace engine::graphics;
        using namespace engine::core;

        Logger::init();

        timeManager = std::make_unique<TimeManager>();
        inputManager = std::make_unique<InputManager>();

        window = std::make_unique<Window>(width, height, "Vulkan", inputManager.get());

        renderer = std::make_unique<Renderer>(width, height, getRequiredExtensions());

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer->getInstance(), window->raw(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer->init(surface);

        // clang-format off
        inputManager->subscribe(InputType::Key, GLFW_KEY_W, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().forward = -1.0f;
        });
        inputManager->subscribe(InputType::Key, GLFW_KEY_W, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().forward = 0.0f;
        });

        inputManager->subscribe(InputType::Key, GLFW_KEY_A, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().right = -1.0f;
        });
         inputManager->subscribe(InputType::Key, GLFW_KEY_A, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().right = 0.0f;
        });

        inputManager->subscribe(InputType::Key, GLFW_KEY_S, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().forward = 1.0f;
        });
        inputManager->subscribe(InputType::Key, GLFW_KEY_S, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().forward = 0.0f;
        });
        
        inputManager->subscribe(InputType::Key, GLFW_KEY_D, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().right = 1.0f;
        });
        inputManager->subscribe(InputType::Key, GLFW_KEY_D, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().getInput().right = 0.0f;
        });

        inputManager->subscribe([&](double x, double y, double deltaX, double deltaY) { 
            renderer->getCamera().rotate(deltaX, deltaY);
        });
        // clang-format on
    }

    Engine::~Engine() {
        inputManager.reset();
        renderer.reset();
        window.reset();
    }

    void Engine::run() {
        using namespace engine::core;

        LOG_INFO("Starting main loop");

        using clock = std::chrono::high_resolution_clock;
        auto lastTime = clock::now();

        while (window->isRunning()) {
            auto now = clock::now();
            timeManager->update(std::chrono::duration<float>(now - lastTime).count());
            lastTime = now;

            window->poll();

            renderer->beginFrame();
            renderer->recordFrame(timeManager->getDeltaTime());
            renderer->endFrame();

            window->present();
        }

        LOG_INFO("Exiting main loop");
    }

    std::vector<const char*> Engine::getRequiredExtensions() const {
        using namespace engine::core;
        using namespace engine::utils;

        uint32_t count;
        const char** ext = glfwGetRequiredInstanceExtensions(&count);

        std::vector<const char*> extensions(ext, ext + count);
        LOG_TRACE("Found '{}' required instance extensions! {}", extensions.size(), string::join(extensions));

        return extensions;
    }

}