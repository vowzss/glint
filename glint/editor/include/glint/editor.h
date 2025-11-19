#pragma once

#include "glint/models/config/resolution_info.h"

struct GLFWwindow;

namespace glint {
    class renderer;

    class editor {
      private:
        resolution_info res_;

        GLFWwindow* window_ = nullptr;
        renderer* renderer_ = nullptr;

      public:
        editor(const resolution_info& res);
        ~editor();

        void run();

      private:
        void createWindow();
        void createRenderer();
    };
}