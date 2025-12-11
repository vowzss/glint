#pragma once

#include "glint/Engine.h"

namespace glint {

    class Editor {
        std::unique_ptr<Engine> engine = nullptr;

      public:
        Editor() = delete;
        Editor(int width, int height);

        ~Editor();

        void run();
    };

}