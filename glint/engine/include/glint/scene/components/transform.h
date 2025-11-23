#pragma once

#include "glint/core/math/quaternion.h"
#include "glint/core/math/vector/vec3.h"

namespace glint::engine::scene::components {
    using vec3 = glint::engine::core::math::vector::vec3;
    using quaternion = glint::engine::core::math::quaternion;

    struct transform {
        vec3 position{};
        quaternion rotation{};
        vec3 scale = vec3::one();
    };

}