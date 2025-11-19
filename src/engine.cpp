
#include "glint/engine.h"

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "glint/renderer.h"

using namespace glint;

engine::engine(const resolution_info& res)
    : res_(res) {
    window_ = createWindow();

    renderer_ = createRenderer();
    renderer_->init(window_);
}

engine::~engine() {
    delete renderer_;
    renderer_ = nullptr;

    glfwDestroyWindow(window_);
    glfwTerminate();
}

void engine::run() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            exit(0);
        }

        renderer_->draw();
    }
}

GLFWwindow* engine::createWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW | failed to initialize!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* w = glfwCreateWindow(res_.width, res_.height, "Vulkan Window", nullptr, nullptr);

    if (w == nullptr) {
        throw std::runtime_error("GLFW | failed to create window!");
    }

    return w;
}

renderer* engine::createRenderer() {
    uint32_t extensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);

    if (validationLayersEnabled) {
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return new renderer(res_, extensions);
}