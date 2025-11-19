#pragma once

#include "models/config/resolution_info.h"

struct GLFWwindow;

namespace glint {
    class renderer;

    class engine {
      private:
        resolution_info res_;

        GLFWwindow* window_ = nullptr;
        renderer* renderer_ = nullptr;

      public:
        engine(const resolution_info& res);
        ~engine();

        void run();

      private:
        GLFWwindow* createWindow();
        renderer* createRenderer();
    };
}