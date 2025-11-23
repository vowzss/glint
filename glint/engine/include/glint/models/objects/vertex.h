#pragma once

#include "glint/core/math/vectors/vec3.h"

struct alignas(16) vertex {
    vec3 pos;
    vec3 color;
    vec3 normal;
    vec2 uv;

    vertex(vec3 p, vec3 c, vec3 n, vec2 t) : pos(p), color(c), normal(n), uv(t) {}
};