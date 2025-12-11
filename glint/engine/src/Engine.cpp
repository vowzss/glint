#include <stdexcept>

#include "glint/Engine.h"
#include "glint/core/Logger.h"
#include "glint/core/managers/AssetManager.h"
#include "glint/core/managers/InputManager.h"
#include "glint/core/managers/TimeManager.h"
#include "glint/core/systems/CameraSystem.h"
#include "glint/graphics/Renderer.h"
#include "glint/graphics/Window.h"
#include "glint/graphics/layers/SceneLayer.h"
#include "glint/scene/World.h"
#include "glint/scene/components/GeometryComponent.h"
#include "glint/utils/FileUtils.h"
#include "glint/utils/StringUtils.h"

// todo: cleanup
#include <GLFW/glfw3.h>

using namespace glint::engine;
using namespace graphics;
using namespace core;
using namespace scene;
using namespace utils;

namespace glint {

    namespace {
        const std::vector<const char*> getRequiredExtensions() {
            uint32_t count;
            const char** ext = glfwGetRequiredInstanceExtensions(&count);

            std::vector<const char*> extensions(ext, ext + count);

#if defined(PLATFORM_MACOS)
            extensions.push_back("VK_KHR_portability_enumeration");
#endif

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
        cameras = std::make_unique<CameraSystem>();

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(renderer->getInstance(), window->raw(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan | failed to create surface!");
        }

        renderer->init(surface);
        renderer->append(std::make_unique<SceneLayer>(*world));

        // todo: cleanup
        // clang-format off
        inputs->subscribe(InputType::Key, GLFW_KEY_W, InputAction::Held, [&](int code, InputAction action) {
            cameras->forward(cameras->active(), action == InputAction::Held ? -1.0f : 0.0f);
        });

        inputs->subscribe(InputType::Key, GLFW_KEY_A, InputAction::Held, [&](int code, InputAction action) { 
            cameras->right(cameras->active(), action == InputAction::Held ? -1.0f : 0.0f);
        });

        inputs->subscribe(InputType::Key, GLFW_KEY_S, InputAction::Held, [&](int code, InputAction action) { 
            cameras->forward(cameras->active(), action == InputAction::Held ? 1.0f : 0.0f);
        });
        
        inputs->subscribe(InputType::Key, GLFW_KEY_D, InputAction::Held, [&](int code, InputAction action) { 
            cameras->right(cameras->active(), action == InputAction::Held ? 1.0f : 0.0f);
        });

        inputs->subscribe([&](double x, double y, double deltaX, double deltaY) { 
            cameras->rotate(cameras->active(), static_cast<float>(deltaX), static_cast<float>(deltaY));
        });
        // clang-format on

        // todo: remove (debug only)
        EntityHandle entity = world->createEntity();
        if (!entity.valid()) return;

        GeometryHandle geometry = world->createGeometry(renderer->getDevices(), files::getModelPath("cube.obj"));
        if (!geometry.valid()) return;

        world->attach(entity, GeometryComponent{geometry});
    }

    Engine::~Engine() = default;

    void Engine::run() {
        LOG_INFO("Starting main loop");

        using clock = std::chrono::high_resolution_clock;
        auto lastTime = clock::now();

        while (window->isRunning()) {
            auto now = clock::now();
            time->tick(std::chrono::duration<float>(now - lastTime).count());
            lastTime = now;

            window->poll();

            renderer->begin();
            renderer->record(time->deltaTime(), cameras->snapshot());
            renderer->end();

            window->present();
        }

        LOG_INFO("Exiting main loop");
    }

}
