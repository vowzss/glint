#include <iostream>

#include "glint/Editor.h"

int main() {
    glint::Editor e(1200, 700);

    try {
        e.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
