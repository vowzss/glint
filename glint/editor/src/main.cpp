#include <iostream>

#include "glint/editor.h"

using namespace glint;

int main() {
    editor e(resolution_info{});

    try {
        e.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
