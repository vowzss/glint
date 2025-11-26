#pragma once

#include <memory>

#include "glint/models/config/resolution_info.h"
#include "glint/window.h"

struct GLFWwindow;

namespace glint::engine::graphics {
    class renderer;
}

namespace glint {
    class editor {
      private:
        resolution_info res_;

        std::unique_ptr<Window> window = nullptr;
        glint::engine::graphics::renderer* renderer = nullptr;

      public:
        editor(const resolution_info& res);
        ~editor();

        void run();

      private:
        void createWindow();
        void createRenderer();
    };
}