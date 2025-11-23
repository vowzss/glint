#pragma once

#include "glint/core/math/quaternion.h"
#include "glint/core/math/vectors/vec3.h"

struct transform {
    vec3 position{};
    quaternion rotation{};
    vec3 scale = vec3::one();
};
