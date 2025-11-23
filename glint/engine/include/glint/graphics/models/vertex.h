#pragma once

#include "glint/core/math/vector/vec3.h"

namespace glint::engine::graphics::models {
    using vec2 = glint::engine::core::math::vector::vec2;
    using vec3 = glint::engine::core::math::vector::vec3;

    struct alignas(16) vertex {
        vec3 pos;
        vec3 color;
        vec3 normal;
        vec2 uv;

        vertex(vec3 p, vec3 c, vec3 n, vec2 t) : pos(p), color(c), normal(n), uv(t) {}
    };
}