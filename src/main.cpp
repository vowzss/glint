#include <iostream>

#include "glint/engine.h"

using namespace glint;

int main() {
    engine e(resolution_info{});

    try {
        e.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
