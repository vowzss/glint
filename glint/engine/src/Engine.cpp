#include <stdexcept>

#include "glint/Engine.h"
#include "glint/core/AssetManager.h"
#include "glint/core/InputManager.h"
#include "glint/core/Logger.h"
#include "glint/core/TimeManager.h"
#include "glint/graphics/Renderer.h"
#include "glint/graphics/Window.h"
#include "glint/graphics/backend/buffer/UniformBuffer.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/scene/World.h"
#include "glint/scene/components/Camera.h"
#include "glint/utils/StringUtils.h"


// todo: cleanup
#include <GLFW/glfw3.h>

using namespace glint::engine;
using namespace graphics;
using namespace layers;
using namespace core;
using namespace scene;

namespace glint {
    namespace {
        const std::vector<const char*> getRequiredExtensions() {
            using namespace engine::utils;

            uint32_t count;
            const char** ext = glfwGetRequiredInstanceExtensions(&count);

            std::vector<const char*> extensions(ext, ext + count);
            LOG_TRACE("Found '{}' required instance extensions! {}", extensions.size(), string::join(extensions));

            return extensions;
        }
    }

    Engine::Engine(int width, int height) {
        Logger::init();

        time = std::make_unique<TimeManager>();
        inputs = std::make_unique<InputManager>();
        assets = std::make_unique<AssetManager>();

        window = std::make_unique<Window>(width, height, "Vulkan", inputs.get());
        renderer = std::make_unique<Renderer>(width, height, getRequiredExtensions());

        world = std::make_unique<World>(assets.get());

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer->getInstance(), window->raw(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer->init(surface);
        renderer->append(std::make_unique<SceneLayer>(*world));

        // clang-format off
        inputs->subscribe(InputType::Key, GLFW_KEY_W, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().forward(-1.0f);
        });
        inputs->subscribe(InputType::Key, GLFW_KEY_W, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().forward(0.0f);
        });

        inputs->subscribe(InputType::Key, GLFW_KEY_A, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().right(-1.0f);
        });
         inputs->subscribe(InputType::Key, GLFW_KEY_A, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().right(0.0f);
        });

        inputs->subscribe(InputType::Key, GLFW_KEY_S, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().forward(1.0f);
        });
        inputs->subscribe(InputType::Key, GLFW_KEY_S, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().forward(0.0f);
        });
        
        inputs->subscribe(InputType::Key, GLFW_KEY_D, InputAction::Held, [&](int code, InputAction action) { 
            renderer->getCamera().right(1.0f);
        });
        inputs->subscribe(InputType::Key, GLFW_KEY_D, InputAction::Released, [&](int code, InputAction action) { 
            renderer->getCamera().right(0.0f);
        });

        inputs->subscribe([&](double x, double y, double deltaX, double deltaY) { 
            renderer->getCamera().rotate(deltaX, deltaY);
        });
        // clang-format on
    }

    Engine::~Engine() = default;

    void Engine::run() {
        using namespace engine::core;

        LOG_INFO("Starting main loop");

        using clock = std::chrono::high_resolution_clock;
        auto lastTime = clock::now();

        while (window->isRunning()) {
            auto now = clock::now();
            time->tick(std::chrono::duration<float>(now - lastTime).count());
            lastTime = now;

            window->poll();

            renderer->beginFrame();
            renderer->recordFrame(time->getDeltaTime());
            renderer->endFrame();

            window->present();
        }

        LOG_INFO("Exiting main loop");
    }

}