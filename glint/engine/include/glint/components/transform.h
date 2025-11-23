#pragma once

#include "glint/core/math/vectors/vec3.h"

struct transform {
    vec3 position{0.0f, 0.0f, 0.0f};
    vec3 rotation{0.0f, 0.0f, 0.0f};
    vec3 scale{1.0f, 1.0f, 1.0f};
};
