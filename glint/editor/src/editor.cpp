#include "glint/Editor.h"

namespace glint {
    Editor::Editor(int width, int height) {
        engine = std::make_unique<Engine>(width, height);
    }

    Editor::~Editor() {
        engine.reset();
    }

    void Editor::run() {
        engine->run();
    }
}