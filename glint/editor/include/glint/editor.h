#pragma once

#include <memory>

#include "glint/Engine.h"

namespace glint {

    struct Editor {
      private:
        std::unique_ptr<Engine> engine = nullptr;

      public:
        Editor() = delete;
        Editor(int width, int height);

        ~Editor();

        void run();
    };

}