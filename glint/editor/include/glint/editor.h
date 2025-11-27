#pragma once

#include <memory>

#include "glint/graphics/Renderer.h"
#include "glint/window.h"

namespace glint {
    struct Editor {
      private:
        std::unique_ptr<Window> window = nullptr;
        std::unique_ptr<engine::graphics::Renderer> renderer = nullptr;

      public:
        Editor() = delete;
        Editor(int width, int height);

        ~Editor();

        void run();

      private:
        std::vector<const char*> getRequiredExtensions() const;
    };
}