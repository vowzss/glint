struct mesh;

#include "glint/core/math/matrix/mat4.h"

struct entity {
    uint32_t id;

    mesh* geometry = nullptr;
    mat4 transform = mat4::identity();
};
